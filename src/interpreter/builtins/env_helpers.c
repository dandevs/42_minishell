/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:40:26 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 17:40:27 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "minishell.h"

int	name_matches(char *entry, char *name)
{
	size_t	len;

	len = ft_strlen(name);
	if (ft_strncmp(entry, name, len) != 0)
		return (0);
	return (entry[len] == '=' || entry[len] == '\0');
}

static int	env_index(char **envp, char *name)
{
	int	i;

	if (!envp || !name)
		return (-1);
	i = 0;
	while (envp[i])
	{
		if (name_matches(envp[i], name))
			return (i);
		i++;
	}
	return (-1);
}

static int	env_add(t_shell *shell, char *entry)
{
	char	**new;
	int		count;

	count = ptrptr_count((void **)shell->envp);
	new = malloc(sizeof(char *) * (count + 2));
	if (!new)
		return (1);
	ft_memcpy(new, shell->envp, sizeof(char *) * count);
	new[count] = entry;
	new[count + 1] = NULL;
	free(shell->envp);
	shell->envp = new;
	return (0);
}

int	env_set_entry(t_shell *shell, char *entry)
{
	char	*name;
	char	*eq;
	char	*dup;
	int		idx;

	eq = ft_strchr(entry, '=');
	name = ft_substr(entry, 0, (unsigned int)(eq - entry));
	if (!name)
		return (1);
	idx = env_index(shell->envp, name);
	free(name);
	dup = ft_strdup(entry);
	if (!dup)
		return (1);
	if (idx >= 0)
	{
		free(shell->envp[idx]);
		shell->envp[idx] = dup;
		return (0);
	}
	return (env_add(shell, dup));
}

int	env_declare(t_shell *shell, char *name)
{
	char	*dup;
	int		idx;

	idx = env_index(shell->envp, name);
	if (idx >= 0)
		return (0);
	dup = ft_strdup(name);
	if (!dup)
		return (1);
	return (env_add(shell, dup));
}
