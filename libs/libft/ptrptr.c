/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptrptr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 13:33:09 by mat               #+#    #+#             */
/*   Updated: 2026/06/28 01:42:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ptrptr_count(void **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i] != NULL)
		i++;
	return (i);
}

int	ptrptr_free(void **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (1);
	while (tab[i] != NULL)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (1);
}
