/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:38:35 by mavallee          #+#    #+#             */
/*   Updated: 2024/06/01 16:07:09 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	temp;

	i = 0;
	temp = -1;
	while (s[i])
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
			temp = i;
		i++;
	}
	if (((unsigned char *)s)[i] == (unsigned char)c)
		temp = i;
	if (temp >= 0)
		return ((char *)(s + temp));
	return (NULL);
}
