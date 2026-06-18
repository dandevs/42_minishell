/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 19:30:04 by marvin            #+#    #+#             */
/*   Updated: 2026/04/26 19:30:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokens_get(t_shell *shell, char *line)
{
	t_tokens	*tmp;
	int			i;

	i = 0;
	tmp = NULL;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (line[i])
		{
			tmp = tokens_values(tmp, line, i);
			if (!tmp || !tmp->lexeme || ft_strcmp(tmp->lexeme, "") == 0)
			{
				cleanup_tokens(shell);
				return (0);
			}
			if (!shell->tokens)
				shell->tokens = tmp;
			i += ft_strlen(tmp->lexeme);
		}
	}
	tmp->next = NULL;
	return (1);
}
