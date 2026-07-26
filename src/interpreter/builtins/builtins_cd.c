/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:40:08 by danimend          #+#    #+#             */
/*   Updated: 2026/07/26 02:45:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "minishell.h"

static char	*resolve_cd_target(t_shell *shell, char **args)
{
	char	*target;

	target = args[1];
	if (!target)
	{
		target = envp_value("HOME", NULL, shell->envp);
		if (!target)
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
	}
	return (target);
}

static int	change_and_get_new(char *target, char *new, size_t size)
{
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (!getcwd(new, size))
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

int	builtin_cd(t_shell *shell, char **args)
{
	char	*target;
	char	old[4096];
	char	new[4096];

	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	target = resolve_cd_target(shell, args);
	if (!target)
		return (1);
	if (!getcwd(old, sizeof(old)))
	{
		perror("minishell: cd");
		return (EXIT_FAILURE);
	}
	if (change_and_get_new(target, new, sizeof(new)))
		return (1);
	set_kv(shell, "OLDPWD", old);
	set_kv(shell, "PWD", new);
	return (EXIT_SUCCESS);
}
