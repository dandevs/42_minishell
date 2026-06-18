/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:52:42 by mavallee          #+#    #+#             */
/*   Updated: 2024/06/01 15:53:48 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	n;
	int	sign;

	i = 0;
	n = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	if (nptr[i] == '+' && sign != -1)
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		n *= 10;
		n = n + sign * (nptr[i] - '0');
		i++;
	}
	return (n);
}
