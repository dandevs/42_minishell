/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 16:09:56 by danimend          #+#    #+#             */
/*   Updated: 2026/08/11 03:12:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

static int	traverse(t_ast *ast, int fd_read, int fd_write,
				t_interpreter_context *ctx)
{
	int	fd[2];

	if (ast->ast_type == AST_PIPE)
	{
		pipe(fd);
		ctx->fd_arr[ctx->fd_len++] = fd[1];
		ctx->fd_arr[ctx->fd_len++] = fd[0];
		if (ast->left != NULL)
			traverse(ast->left, fd_read, fd[1], ctx);
		if (fd[1] != STDOUT_FILENO)
			close(fd[1]);
		if (ast->right != NULL)
			traverse(ast->right, fd[0], fd_write, ctx);
		if (fd[0] != STDIN_FILENO)
			close(fd[0]);
	}
	else if (ast->ast_type == AST_CMD)
	{
		run_cmd_child(ast, fd_read, fd_write, ctx);
	}
	return (1);
}

static int	try_builtin_only(t_shell *shell, t_interpreter_context *ctx,
		t_interpreter_result *result)
{
	t_builtin_fn	fn;

	if (shell->ast && shell->ast->ast_type == AST_CMD
		&& shell->ast->args && shell->ast->args[0])
	{
		fn = get_builtin(shell->ast->args[0]);
		if (fn)
		{
			result->exit_status = run_builtin_parent(ctx, shell->ast, fn);
			result->signal = 0;
			return (1);
		}
	}
	return (0);
}

static void	wait_and_collect_result(t_interpreter_context *ctx,
		t_interpreter_result *result)
{
	int	i;
	int	status;

	i = 0;
	while (i < ctx->pid_len)
	{
		waitpid(ctx->pid_arr[i], &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE)
			ft_putstr_fd(" Broken pipe\n", 2);
		if (ctx->pid_arr[i] == ctx->pid_arr[ctx->pid_len - 1])
		{
			if (WIFSIGNALED(status))
			{
				result->exit_status = EX_SIGNAL_BASE + WTERMSIG(status);
				result->signal = WTERMSIG(status);
				return ;
			}
			result->exit_status = WEXITSTATUS(status);
			result->signal = 0;
		}
		i++;
	}
}

t_interpreter_result	interpret(t_shell *shell)
{
	t_interpreter_context	context;
	t_interpreter_result	result;

	context.shell = shell;
	result.exit_status = 0;
	result.signal = 0;
	context.pid_len = 0;
	context.fd_len = 0;
	if (try_builtin_only(shell, &context, &result))
		return (result);
	traverse(shell->ast, STDIN_FILENO, STDOUT_FILENO, &context);
	if (context.pid_len == 0)
		return (result);
	wait_and_collect_result(&context, &result);
	return (result);
}
