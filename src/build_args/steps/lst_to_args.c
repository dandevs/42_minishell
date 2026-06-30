/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 19:31:22 by marvin            #+#    #+#             */
/*   Updated: 2026/06/23 19:31:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lst_to_args(t_list *lst, t_ast *ast)
{
	t_list	*tmp;
	int		i;

	ast->args = malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	if (!ast->args)
		return (ft_lstclear(&lst, free), 0);
	i = 0;
	tmp = lst;
	while (tmp)
	{
		ast->args[i] = (char *)tmp->content;
		i++;
		tmp = tmp->next;
	}
	ast->args[i] = NULL;
	ft_lstclear(&lst, NULL);
	return (1);
}
