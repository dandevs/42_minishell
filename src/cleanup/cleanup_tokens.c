/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 21:42:37 by marvin            #+#    #+#             */
/*   Updated: 2026/06/06 21:42:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_tokens *tokens)
{
	t_tokens	*tmp;
	t_tokens	*tmpb;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->lexeme)
			free(tmp->lexeme);
		tmpb = tmp;
		tmp = tmpb->next;
		free(tmpb);
	}
}

void	cleanup_tokens(t_shell *shell)
{
	if (shell->tokens)
		free_tokens(shell->tokens);
	shell->tokens = NULL;
}

