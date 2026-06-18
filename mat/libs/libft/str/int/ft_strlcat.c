/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:47:01 by mavallee          #+#    #+#             */
/*   Updated: 2024/06/01 16:06:04 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	j;
	size_t	dst_len;

	i = 0;
	j = 0;
	dst_len = ft_strlen(dst);
	while (dst[i] && siz > i + 1)
	{
		j++;
		i++;
	}
	while (i + 1 < siz && src[i - j])
	{
		dst[i] = src[i - j];
		i++;
	}
	if (siz > dst_len)
		dst[i] = '\0';
	if (siz < dst_len)
		return ((size_t)(ft_strlen(src) + siz));
	else
		return ((size_t)(ft_strlen(src) + dst_len));
}
/*
#include <stdio.h>

int	main(void)
{
	char	dest[30];
	int		a;

	char * src = (char *)"AAAAAAAAA";
	ft_memset(dest, 'C', 5);
	printf("%s \n", dest);
	a = (int)(ft_strlcat(dest, src, -1));
	printf("%d \n", a);
	printf("%s \n", dest);
}
*/
