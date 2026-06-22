/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_replace.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 19:48:57 by marvin            #+#    #+#             */
/*   Updated: 2026/06/22 19:48:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*str_replace2(char *str, char *dest, char *old, char *new)
{
	int	done;
	int	i;
	int	j;

	done = 0;
	i = 0;
	j = 0;
	while (str && str[i])
	{
		if (!done && ft_strncmp(str + i, old, ft_strlen(old)) == 0)
		{
			dest[j] = '\0';
			ft_strlcat(dest, new, ft_strlen(new) + ft_strlen(dest) + 1);
			j += ft_strlen(new);
			i += ft_strlen(old);
			done = 1;
			continue ;
		}
		dest[j] = str[i];
		j++;
		i++;
	}
	dest[j] = '\0';
	return (dest);
}

char	*str_replace(char *str, char *old, char *new)
{
	char	*dest;

	if (!ft_strnstr(str, old, ft_strlen(str)))
		return (ft_strdup(str));
	dest = malloc(sizeof(char) * (ft_strlen(str) - ft_strlen(old) + ft_strlen(new) + 1));
	if (!dest)
		return (NULL);
	str_replace2(str, dest, old, new);
	return (dest);
}
/*
int	main(void)
{
	char	*c = "$ok mais t folle meuf";

	char	*replace = str_replace(c, "mais", NULL);
	printf("%s\n", replace);
	free(replace);
	return (1);
}*/
