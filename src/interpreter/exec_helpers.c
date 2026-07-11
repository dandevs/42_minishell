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

#include "interpreter.h"

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
	int			i;
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

char	*resolve_path(char *name, char **envp)
{
	char	**dirs;
	char	*found;

	if (!name || !*name)
		return (NULL);
	if (has_slash(name))
		return (ft_strdup(name));
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

void	run_cmd(t_ast *cmd, int fd_read, int fd_write,
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
		close_inherited_fds();
		exec_child(cmd, ctx);
	}
}
