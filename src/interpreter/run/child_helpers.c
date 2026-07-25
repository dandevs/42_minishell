/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 16:29:02 by danimend          #+#    #+#             */
/*   Updated: 2026/07/26 00:21:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

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

static char	*resolve_path(char *name, char **envp)
{
	char	**dirs;
	char	*found;

	if (!name || !*name)
		return (NULL);
	if (has_slash(name))
	{
		if (access(name, F_OK) == 0)
			return (ft_strdup(name));
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (NULL);
	}
	dirs = get_path_dirs(name, envp);
	if (!dirs)
		return (NULL);
	found = search_path_dirs(dirs, name);
	if (!found)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	return (found);
}

static void	test_access(char *path)
{
	struct stat		buf;

	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		free(path);
		exit(126);
	}
	if (stat(path, &buf) != 0)
	{
		free(path);
		perror("minishell");
		exit(127);
	}
	if (S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
		free(path);
		exit(126);
	}
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
	test_access(path);
	execve(path, cmd->args, ctx->shell->envp);
	free(path);
	perror("minishell: ");
	exit(127);
}
