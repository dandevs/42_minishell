/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 23:54:38 by marvin            #+#    #+#             */
/*   Updated: 2026/06/22 23:54:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*on doit : expand chaque token, word split*/

int	new_arg(t_list *node, char *content, int i)
{
	t_list	*new;
	char	*trimmed_content;

	new = ft_lstnew((void *)ft_substr(content, i + 1,
				ft_strlen(content + i + 1)));
	if (!new)
		return (0);
	if (!new->content)
		return (free(new), 0);
	new->next = node->next;
	node->next = new;
	trimmed_content = realloc(node->content, sizeof(char) * i + 1);
	if (!trimmed_content)
		return (0);
	node->content = (void *)trimmed_content;
	((char *)node->content)[i] = '\0';
	return (1);
}

int	split_args(t_list *node)
{
	t_mode	mode;
	char	*content;
	int		i;

	mode = NORMAL;
	while (node)
	{
		content = (char *)node->content;
		i = 0;
		while (content[i])
		{
			mode_change(&mode, content, i);
			if (mode == NORMAL && ft_isspace(content[i]))
			{
				if (new_arg(node, content, i))
					break ;
				return (0);
			}
			else
				i++;
		}
		node = node->next;
	}
	return (1);
}

int	remove_blanks(t_list **lst, t_list *node)
{
	t_list	*next;

	if (node)
		next = node->next;
	while (node)
	{
		if (node == *lst && ((char *)node->content)[0] == '\0')
		{
			*lst = node->next;
			ft_lstdelone(node, free);
			node = *lst;
		}
		else if (next && ((char *)next->content)[0] == '\0')
		{
			node->next = next->next;
			ft_lstdelone(next, free);
		}
		else
			node = node->next;
		if (node)
			next = node->next;
	}
	return (1);
}

int	remove_quotes(t_list *node)
{
	t_mode	mode;
	int		i;

	mode = NORMAL;
	while (node)
	{
		i = 0;
		while (((char *)node->content)[i])
		{
			if (mode_change(&mode, ((char *)node->content), i))
				ft_memmove(node->content + i, node->content + i + 1,
					1 + ft_strlen((char *)node->content + i));
			else
				i++;
		}
		node = node->next;
	}
	return (1);
}

int	finalize_lst(t_list **lst)
{
	if (!split_args(*lst))
		return (ft_lstclear(lst, free), 0);
	if (!remove_blanks(lst, *lst) || !remove_quotes(*lst))
		return (ft_lstclear(lst, free), 0);
	return (1);
}
