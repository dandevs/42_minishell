/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:13:07 by danimend          #+#    #+#             */
/*   Updated: 2026/07/25 22:10:04 by marvin           ###   ########.fr       */
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
	char	*tmp;
	char	*candidate;
	int		i;

	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		if (!tmp)
			break ;
		candidate = ft_strjoin(tmp, name);
		free(tmp);
		if (!candidate)
			break ;
		if (access(candidate, X_OK) == 0)
		{
			ptrptr_free((void **)dirs);
			return (candidate);
		}
		free(candidate);
		i++;
	}
	ptrptr_free((void **)dirs);
	return (NULL);
}
