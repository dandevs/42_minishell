/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 19:24:46 by marvin            #+#    #+#             */
/*   Updated: 2026/04/26 19:24:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_line(t_shell *shell)
{
	char	*line;

	if (shell->last_line)
		free(shell->last_line);
	shell->last_line = NULL;
	if (shell->line)
	{
		shell->last_line = ft_strdup(shell->line);
		free(shell->line);
		shell->line = NULL;
	}
	if (!isatty(0))
		line = readline(NULL);
	else
		line = readline("minishell :) > ");
	if (isatty(0) && line && *line && !str_empty(line))
	{
		if ((shell->last_line && ft_strcmp(line, shell->last_line) != 0)
			|| !shell->last_line)
			add_history(line);
	}
	return (line);
}
