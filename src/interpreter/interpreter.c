/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 07:14:58 by danimend          #+#    #+#             */
/*   Updated: 2026/06/21 00:48:52 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static int	is_redir(t_token t)
{
	return (t == GREATER || t == DGREATER || t == LESSER || t == DLESSER);
}

static int	count_argv(t_tokens *start, t_tokens *end)
{
	int	count;
	count = 0;

	while (start && start != end && !is_redir(start->token))
	{
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
	i = 0;
	
	if (!argv)
		return (NULL);

	while (start && start != end)
	{
		argv[i++] = start->lexeme;
		start = start->next;
	}

	argv[i] = NULL;
	return (argv);
}

static void	run_cmd(t_ast *cmd, t_interpreter_context *ctx)
{
	int	pid;
	int	fd_read;
	int	fd_write;

	if (ctx->fd_len >= 2)
	{
		fd_write = ctx->fd_arr[ctx->fd_len--];
		fd_read = ctx->fd_arr[ctx->fd_len--];
	}
	else
	{
		fd_write = STDOUT_FILENO;
		fd_read = STDIN_FILENO;
	}

	pid = fork();
	ctx->pid_arr[ctx->pid_len++] = pid;

	if (pid == 0)
	{
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

		execve(cmd->start->lexeme, build_argv(cmd->start, cmd->end), NULL);
		exit(127);
	} 
	
	if (fd_read != STDIN_FILENO)
		close(fd_read);
	if (fd_write != STDOUT_FILENO)
		close(fd_write);
}

static int	traverse(t_ast *ast, t_interpreter_context *ctx)
{
	int	fd[2];

	if (ast->ast_type == AST_PIPE)
	{
		pipe(fd);
		ctx->fd_arr[ctx->fd_len++] = fd[0];
		ctx->fd_arr[ctx->fd_len++] = fd[1];

		if (ast->left != NULL && ast->right != NULL)
		{
			if (ast->left->ast_type == AST_CMD && ast->right->ast_type == AST_CMD)
				ctx->fd_arr[ctx->fd_len++] = STDIN_FILENO;
		}
	}

	if (ast->left != NULL && ast->ast_type == AST_PIPE)
		traverse(ast->left, ctx);

	if (ast->right != NULL && ast->ast_type == AST_PIPE)
		traverse(ast->right, ctx);

	if (ast->ast_type == AST_CMD)
		run_cmd(ast, ctx);

	return (1);
}

t_interpreter_result	interpret(t_ast *ast)
{
	t_interpreter_context	context;
	t_interpreter_result	result;
	context.fd_len = 0;
	context.fd_arr[context.fd_len++] = STDOUT_FILENO;
	traverse(ast, &context);

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
