/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptrptr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 13:33:09 by mat               #+#    #+#             */
/*   Updated: 2026/06/15 13:59:53 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ptrptr_count(void **tab)
{
    int i;

    i = 0;
    if (!tab)
        return (0);
    while(tab[i] != NULL)
        i++;
    return (i);
}

int ptrptr_free(void **tab)
{
    int i;

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
