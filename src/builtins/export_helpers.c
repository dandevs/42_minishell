/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:40:30 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 17:40:32 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "minishell.h"

static void	sort_env(char **env, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(env[i], env[j]) < 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_entry(char *entry)
{
	char	*eq;
	int		i;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	eq = ft_strchr(entry, '=');
	if (!eq)
	{
		ft_putendl_fd(entry, STDOUT_FILENO);
		return ;
	}
	i = 0;
	while (entry + i != eq)
	{
		ft_putchar_fd(entry[i], STDOUT_FILENO);
		i++;
	}
	ft_putchar_fd('=', STDOUT_FILENO);
	ft_putchar_fd('"', STDOUT_FILENO);
	i++;
	while (entry[i])
	{
		ft_putchar_fd(entry[i], STDOUT_FILENO);
		i++;
	}
	ft_putendl_fd("\"", STDOUT_FILENO);
}

int	export_print(char **envp)
{
	char	**copy;
	int		count;
	int		i;

	count = ptrptr_count((void **)envp);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (1);
	i = 0;
	while (i < count)
	{
		copy[i] = envp[i];
		i++;
	}
	copy[count] = NULL;
	sort_env(copy, count);
	i = 0;
	while (i < count)
	{
		print_export_entry(copy[i]);
		i++;
	}
	free(copy);
	return (0);
}
