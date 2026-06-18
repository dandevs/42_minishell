/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:56 by mavallee          #+#    #+#             */
/*   Updated: 2024/05/22 18:14:51 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t siz)
{
	size_t	i;

	i = 0;
	while (i + 1 < siz && siz > 0 && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	if (siz > 0)
		dest[i] = '\0';
	return (ft_strlen(src));
}
