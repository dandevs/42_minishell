/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:38:35 by mavallee          #+#    #+#             */
/*   Updated: 2026/06/14 17:36:32 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	int		i;
	int		j;

	j = 0;
	i = 0;
	res = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!res)
		return (NULL);
	while (s1 && s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = '\0';
	return (res);
}
/*
char	*ft_strjoin(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	j;
	size_t	dst_len;

	i = 0;
	j = 0;
	dst_len = ft_strlen(dst);
	while (dst[i] && siz > i + 1) // incomprehension sujet
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
*/