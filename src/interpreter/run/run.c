/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:14:33 by danimend         #+#    #+#             */
/*   Updated: 2026/07/11 17:17:00 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

int	run_builtin_parent(t_interpreter_context *ctx, t_ast *cmd,
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

void	run_cmd_child(t_ast *cmd, int fd_read, int fd_write,
		t_interpreter_context *ctx)
{
	int	pid;

	pid = fork();
	ctx->pid_arr[ctx->pid_len++] = pid;
	if (pid == 0)
	{
		reset_signals_default();
		setup_child_fds(fd_read, fd_write);
		if (!process_redirs(cmd, apply_one_redir, apply_one_heredoc))
			exit(1);
		close_inherited_fds(ctx, fd_read, fd_write);
		exec_child(cmd, ctx);
	}
}
