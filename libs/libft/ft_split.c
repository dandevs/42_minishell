/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:24:15 by mavallee          #+#    #+#             */
/*   Updated: 2026/03/20 19:22:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	valeurs return countstr :
		-1 : erreur // AUCUN MOT
		count : nb de chaines a stock ds tab_str
		?-> count = 0 tableau avc juste null ? "empty" != ""
*/

static int	ft_countstr(char const *s, char c)
{
	int	i;
	int	count;
	int	j;

	if (!s)
		return (-1);
	i = 0;
	count = 0;
	while (s[i])
	{
		j = 0;
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i])
		{
			while (s[i + j] && s[i + j] != c)
				j++;
			count++;
			i = i + j;
		}
		if (s[i])
			i++;
	}
	return (count);
}

/*
valeur return ft_splitstr:
	-1 : erreur // stopper les tests
	j : position a laquelle refaire le meme test)
	  -> si j = 0 (soit "l.34: return 0") derniere chaine (stopper les test)
*/

static int	ft_splitstr(char const *s, char c, int i)
{
	int	j;

	if (!s[i] || s[i] == c)
		return (0);
	j = 0;
	while (s[i + j] && s[i + j] != c)
		j++;
	if (s[i + j] == c || !s[i + j])
		return (j);
	return (0);
}

int	free_all(char **tab_str, int k, int i, int j)
{
	int	l;

	tab_str[k] = malloc(sizeof(char) * (((i + j) - i) + 1));
	if (!tab_str[k])
	{
		l = 0;
		while (tab_str[l])
		{
			free(tab_str[l]);
			l++;
		}
		free(tab_str);
		return (1);
	}
	return (0);
}

int	maj(int i, const char *s, char c)
{
	while (s[i] == c)
		i++;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		k;
	char	**tab_str;

	tab_str = malloc(sizeof(char *) * (ft_countstr(s, c)) + sizeof(NULL));
	if (!tab_str || ft_countstr(s, c) < 0)
	{
		return (NULL);
	}
	tab_str[ft_countstr(s, c)] = NULL;
	k = 0;
	i = 0;
	while (s[i] && k != ft_countstr(s, c))
	{
		i = maj(i, s, c);
		if (free_all(tab_str, k, i, ft_splitstr(s, c, i)))
		{
			return (NULL);
		}
		ft_strlcpy(tab_str[k], (char *)(s + i), ((i + ft_splitstr(s, c, i)) - i)
			+ 1);
		k++;
		i += ft_splitstr(s, c, i);
	}
	return (tab_str);
}

/*	SPLIT OG HERE (fonctionne mais trop long)

char	**ft_split(char const *s, char c)
{
int			i;
int			j;
int			k;
int			l;
char		**tab_str;

if	(ft_countstr(s, c) < 0)
{
	return (NULL);
}
tab_str = malloc(sizeof(char *) * (ft_countstr(s, c)) + sizeof(NULL));
if (!tab_str || ft_countstr(s, c) < 0)
{
	return (NULL);
}
tab_str[ft_countstr(s, c)] = NULL;
k = 0;
i = 0;
while (s[i] && k != ft_countstr(s, c))
{
	j = 0;
	if ((i == 0 && s[i]) || s[i] == c)
	{
		if (s[i] == c)
			i++;
		j = ft_splitstr(s, c, i);
		if (j != -1)
		{
			tab_str[k] = malloc(sizeof(char) * (((i + j) - i) + 1));
			if (!tab_str[k])
			{
				l = 0;
				while (tab_str[l])
				{
					free(tab_str[l]);
					l++;
				}
				free(tab_str);
				return (NULL);
			}
			ft_strlcpy(tab_str[k], (char *)(s + i), ((i + j) - i) + 1);
			k++;
			i += j;
		}
	}
}
return (tab_str);
}
*/

/*
#include <stdio.h>

int	main(int argc, char **argv)
{
	char	**dest;
	int		i;

	(void)argc;
	printf("%s a comme nb de mots delimites par %c: %d, pos du prochain stop :
		%d\n", argv[1], argv[2][0], ft_countstr(argv[1], argv[2][0]),
		ft_splitstr(argv[1], argv[2][0], 1));
	printf("liste de mots :\n");
	dest = ft_split(argv[1], argv[2][0]);
	printf("dest cree \n");
	i = 0;
	while (dest[i] != NULL)
	{
		printf("%s \n", dest[i]);
		i++;
	}
	if (dest[i] == NULL)
		printf("NULL AJT");
	while (dest[i])
	{
		free(dest);
		i++;
	}
	free(dest);
}
*/
