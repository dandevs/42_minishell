/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 22:19:19 by mat               #+#    #+#             */
/*   Updated: 2026/07/01 01:29:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*envp_value(char *name, char *last_status, char **envp)
{
	int	i;

	i = 0;
	if (name[0] == '?' && last_status)
		return (last_status);
	while (envp && envp[i] && name && *name)
	{
		if (ft_strncmp(envp[i], name, ft_strlen(name)) == 0
			&& envp[i][ft_strlen(name)] == '=')
			return (envp[i] + ft_strlen(name) + 1);
		i++;
	}
	return (NULL);
}

int	mode_change(t_mode *mode, char *word, int i)
{
	if (word[i] == '"' && *mode == NORMAL)
		*mode = DOUBLEQ;
	else if (word[i] == '\'' && *mode == NORMAL)
		*mode = SINGLEQ;
	else if (word[i] == '\'' && *mode == SINGLEQ)
		*mode = NORMAL;
	else if (word[i] == '"' && *mode == DOUBLEQ)
		*mode = NORMAL;
	else
		return (0);
	return (1);
}
