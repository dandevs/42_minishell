/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 16:29:02 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 20:15:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpreter.h"

void	setup_child_fds(int fd_read, int fd_write)
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
}

void	close_inherited_fds(void)
{
	int	i_fd;

	i_fd = 2;
	while (++i_fd < 512)
		close(i_fd);
}

void	exec_child(t_ast *cmd, t_interpreter_context *ctx)
{
	t_builtin_fn	fn;
	char			*path;

	fn = NULL;
	if (cmd->args && cmd->args[0])
		fn = get_builtin(cmd->args[0]);
	if (fn)
		exit(fn(ctx->shell, cmd->args));
	path = resolve_path(cmd->args[0], ctx->shell->envp);
	if (!path)
		exit(127);
	execve(path, cmd->args, ctx->shell->envp);
	free(path);
	exit(127);
}
