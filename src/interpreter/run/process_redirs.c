/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:39:17 by danimend          #+#    #+#             */
/*   Updated: 2026/07/25 22:14:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

int	apply_one_redir(t_tokens *op, t_tokens *file)
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
	if (op->token == LESSER)
		target = STDIN_FILENO;
	else
		target = STDOUT_FILENO;
	dup2(fd, target);
	close(fd);
	return (1);
}

int	process_redirs(t_ast *cmd, t_redir_cb cb, t_heredoc_cb hd_cb)
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
