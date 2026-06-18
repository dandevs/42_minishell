/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:57:00 by mavallee          #+#    #+#             */
/*   Updated: 2024/06/03 19:14:47 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*ptr;

	ptr = *lst;
	if (!lst)
		return ;
	while (*lst)
	{
		ptr = (*lst)->next;
		if (del)
			del((*lst)->content);
		free(*lst);
		*lst = ptr;
	}
	lst = NULL;
}
