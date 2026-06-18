/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 06:00:30 by mavallee          #+#    #+#             */
/*   Updated: 2024/05/27 14:23:17 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_findset(char const *s1, int j, char const *set)
{
	int	test;
	int	i;

	test = 1;
	i = 0;
	if (!set[i])
		return (-1);
	while (set[i] != s1[j] && set[i] && test > 0)
	{
		i++;
		if (!set[i])
			test--;
	}
	return (test);
}

static int	ft_compare(char const *s1, int j, char const *set, int sign)
{
	int	test;

	test = 0;
	while (s1[j] && test == 0)
	{
		if (ft_findset(s1, j, set) == 1)
			j += sign;
		else
			test++;
	}
	return (j);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		k;
	char	*dest;

	i = ft_compare(s1, 0, set, 1);
	j = ft_compare(s1, ft_strlen(s1) - 1, set, -1);
	if (i > j)
	{
		i = 0;
		j = -1;
	}
	dest = malloc(sizeof(char) * (1 + (j - i + 1)));
	if (!dest)
		return (NULL);
	k = 0;
	while (i <= j)
	{
		dest[k] = s1[i];
		i++;
		k++;
	}
	dest[k] = '\0';
	return (dest);
}
/*
#include <stdio.h>

int	main(int argc, char **argv)
{
	if (argc == 3)
	{
			printf("%s : chaine trimee \n", ft_strtrim(argv[1], argv[2]));
			printf("%d \n", ft_compare(argv[1], ft_strlen(argv[1]), argv[2],
					-1));
			printf("%d \n", ft_findset(argv[1], 16, argv[2]));
	}
}
*/