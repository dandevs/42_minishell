/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 07:14:58 by danimend          #+#    #+#             */
/*   Updated: 2026/06/29 18:22:03 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// /bin/ls -l -a | /bin/grep lib | /bin/wc -l
// /bin/ls -l -a | /bin/grep lib | /bin/wc -l
#include "minishell.h"
#include "builtins.h"
#include <fcntl.h>
#include <sys/wait.h>

static int	is_redir(t_token t)
{
	return (t == GREATER || t == DGREATER || t == LESSER || t == DLESSER);
}

static void	skip_redir_token_and_word(t_tokens **head)
{
	*head = (*head)->next;
	if (*head)
		*head = (*head)->next;
}

static int	count_argv(t_tokens *start, t_tokens *end)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (is_redir(start->token))
		{
			skip_redir_token_and_word(&start);
			continue ;
		}
		if (start->token == WORD)
			count++;
		start = start->next;
	}
	return (count);
}

static char	**build_argv(t_tokens *start, t_tokens *end)
{
	char	**argv;
	int		i;

	argv = malloc(sizeof(char *) * (count_argv(start, end) + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (start && start != end)
	{
		if (is_redir(start->token))
		{
			skip_redir_token_and_word(&start);
			continue ;
		}
		if (start->token == WORD)
			argv[i++] = start->lexeme;
		start = start->next;
	}
	argv[i] = NULL;
	return (argv);
}

static int	apply_one_redir(t_tokens *op, t_tokens *file)
{
	int	fd;
	int	target;

	if (op->token == GREATER)
		fd = open(file->lexeme, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (op->token == DGREATER)
		fd = open(file->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (op->token == LESSER)
		fd = open(file->lexeme, O_RDONLY);
	else
		return (1);
	if (fd < 0)
		return (perror(file->lexeme), 0);
	target = (op->token == LESSER) ? STDIN_FILENO : STDOUT_FILENO;
	dup2(fd, target);
	close(fd);
	return (1);
}

static int	apply_one_heredoc(char *hd)
{
	int	p[2];

	if (pipe(p) < 0)
		return (perror("minishell: heredoc"), 0);
	write(p[1], hd, ft_strlen(hd));
	close(p[1]);
	dup2(p[0], STDIN_FILENO);
	close(p[0]);
	return (1);
}

static int	process_redirs(t_ast *cmd, t_redir_cb cb, t_heredoc_cb hd_cb)
{
	int		i;
	t_redirs	*redir;

	i = 0;
	while (cmd->redirs && cmd->redirs[i])
	{
		redir = cmd->redirs[i];
		if (redir->tokens->token == DLESSER)
		{
			if (hd_cb && !hd_cb(redir->hd))
				return (0);
		}
		else if (redir->file && !cb(redir->tokens, redir->file))
			return (0);
		i++;
	}
	return (1);
}

static int	run_builtin_parent(t_interpreter_context *ctx, t_ast *cmd,
				t_builtin_fn fn)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in < 0 || saved_out < 0)
		return (1);
	if (!process_redirs(cmd, apply_one_redir, apply_one_heredoc))
	{
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		return (1);
	}
	status = fn(ctx->shell, cmd->args);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (status);
}

static void	run_cmd(t_ast *cmd, int fd_read, int fd_write, t_interpreter_context *ctx)
{
	int				pid;
	t_builtin_fn	fn;

	pid = fork();
	ctx->pid_arr[ctx->pid_len++] = pid;

	if (pid == 0)
	{
		reset_signals_default();
		if (fd_read != STDIN_FILENO)
		{
			dup2(fd_read, STDIN_FILENO);
			close(fd_read);
		}

		if (fd_write != STDOUT_FILENO)
		{
			dup2(fd_write, STDOUT_FILENO);
			close(fd_write);
		}

		if (!process_redirs(cmd, apply_one_redir, apply_one_heredoc))
			exit(1);

		for (int i_fd = 3; i_fd < 512; i_fd++)
			close(i_fd);

		fn = NULL;
		if (cmd->args && cmd->args[0])
			fn = get_builtin(cmd->args[0]);
		if (fn)
			exit(fn(ctx->shell, cmd->args));
		execve(cmd->start->lexeme, build_argv(cmd->start, cmd->end), ctx->shell->envp);
		exit(127);
	}
}

static int	traverse(t_ast *ast, int fd_read, int fd_write, t_interpreter_context *ctx)
{
	int	fd[2];

	if (ast->ast_type == AST_PIPE)
	{
		pipe(fd);
		// printf("pipe created: [%d, %d]\n", fd[0], fd[1]);

		if (ast->left != NULL)
			traverse(ast->left, fd_read, fd[1], ctx);
		
		if (ast->right != NULL)
			traverse(ast->right, fd[0], fd_write, ctx);
		
		if (fd[0] != STDIN_FILENO)
			close(fd[0]);

		if (fd[1] != STDOUT_FILENO)
			close(fd[1]);
	}
	else if (ast->ast_type == AST_CMD)
	{
		// printf("yep: %s, %d | %d\n", ast->start->lexeme, fd_read, fd_write);
		run_cmd(ast, fd_read, fd_write, ctx);
	}
	 
	return (1);
}

t_interpreter_result	interpret(t_shell *shell)
{
	t_interpreter_context	context;
	t_interpreter_result	result;
	t_builtin_fn			fn;

	context.pid_len = 0;
	context.shell = shell;
	result.exit_status = 0;
	result.signal = 0;
	if (shell->ast && shell->ast->ast_type == AST_CMD
		&& shell->ast->args && shell->ast->args[0])
	{
		fn = get_builtin(shell->ast->args[0]);
		if (fn)
		{
			result.exit_status = run_builtin_parent(&context, shell->ast, fn);
			return (result);
		}
	}
	traverse(shell->ast, STDIN_FILENO, STDOUT_FILENO, &context);

	while (context.pid_len > 0)
	{
		int	status;
		int	pid;

		pid = context.pid_arr[--context.pid_len];
		waitpid(pid, &status, 0);

		if (WIFSIGNALED(status))
		{
			return (t_interpreter_result)
			{
				.exit_status = 128 + WTERMSIG(status),
				.signal = WTERMSIG(status)
			};
		}

		result.exit_status = WEXITSTATUS(status);
		result.signal = 0;
	}

	return (result);
}
