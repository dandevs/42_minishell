/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 22:43:23 by mat               #+#    #+#             */
/*   Updated: 2026/06/15 23:19:17 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_lstsplit2(t_list *lst, t_list *new, int i)
{
    char    *content;

    new->next = lst->next;
    lst->next = new;
    ((char *)lst->content)[i] = '\0';
    content = ft_strdup((char *)lst->content);
    if (!content)
        return (0);
    free(lst->content);
    lst->content = (void *)content;
    return (1);
}

int ft_lstsplit(t_list *lst, int i)
{
    t_list  *new;
    char    *tail;

    tail = ft_strdup(((char *)lst->content) + i + 1);
    if (!tail)
        return (0);
    new = ft_lstnew((void *)tail);
    if (!new)
    {
        free(tail);
        return (0);
    }
    if (!ft_lstsplit2(lst, new, i))
        return (0);
    return (1);
}
