/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:13:07 by danimend          #+#    #+#             */
/*   Updated: 2026/07/26 00:49:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

int	has_slash(char *name)
{
	return (name && ft_strchr(name, '/') != NULL);
}

char	**get_path_dirs(char *name, char **envp)
{
	char	*path_env;
	char	**dirs;

	path_env = envp_value("PATH", NULL, envp);
	if (!path_env)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (NULL);
	}
	dirs = ft_split(path_env, ':');
	return (dirs);
}

char	*search_path_dirs(char **dirs, char *name)
{
	char	*candidate;
	char	*backup;
	int		i;

	i = -1;
	backup = NULL;
	while (dirs[++i])
	{
		candidate = ft_strjoin_3(dirs[i], "/", name);
		if (!candidate)
			break ;
		if (access(candidate, F_OK) != 0)
		{
			free(candidate);
			continue ;
		}
		if (backup)
			free(backup);
		backup = ft_strdup(candidate);
		free(candidate);
		if (access(backup, X_OK) != 0)
			continue ;
		break ;
	}
	return (ptrptr_free((void **)dirs), backup);
}
