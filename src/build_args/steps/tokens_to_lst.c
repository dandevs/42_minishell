/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 21:15:35 by marvin            #+#    #+#             */
/*   Updated: 2026/06/23 21:15:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokens_to_lst(t_list **lst, t_tokens *start, t_tokens *end)
{
	t_list	*node;
	char	*content;
	int		redir_file;

	redir_file = 0;
	while (start && start != end)
	{
		if (start->token != WORD || redir_file == 1)
		{
			if (redir_file == 1)
				redir_file = 0;
			else if (start->token != PIPE)
				redir_file = 1;
			start = start->next;
			continue ;
		}
		content = ft_strdup(start->lexeme);
		if (!content)
			return (0);
		node = ft_lstnew((void *)content);
		if (!node)
			return (free(content), 0);
		ft_lstadd_back(lst, node);
		start = start->next;
	}
	return (1);
}
