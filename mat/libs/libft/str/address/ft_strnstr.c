/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:38:35 by mavallee          #+#    #+#             */
/*   Updated: 2024/06/01 17:41:35 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_min(size_t a, size_t b)
{
	if (a < b)
		return (a);
	return (b);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	to_compare;

	i = 0;
	to_compare = ft_min(len, ft_strlen(little));
	if (!little || (!*big && !*little))
		return (&(((char *)big)[i]));
	while (big[i] && (i + to_compare <= len) && (i
			+ to_compare >= ft_strlen(little)))
	{
		if (!ft_strncmp((char *)(big + i), little, to_compare))
			return (&(((char *)big)[i]));
		i++;
	}
	return (NULL);
}

/*
int				i;

i = 0;
if	((int)len < 0)
	len = abs((int)len);
if (!little || (!*big && !*little))
	return (&(((char *)big)[i]));
while (big[i] +)
{
	if (ft_strncmp((char *)(big + i), little, ft_min(len,
				ft_strlen(little))) == 0 && len != 0)
		return (&(((char *)big)[i]));
	i++;
}
return (NULL);
*/
/*
#include <stdio.h>

int	main(void)
{
	char	haystack[30] = "aaabcabcd";
	char	needle[10] = "c";

	printf("%s \n", ft_strnstr(haystack, needle, 1));
}
*/