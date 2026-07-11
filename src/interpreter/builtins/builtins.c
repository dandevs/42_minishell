/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:40:21 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 17:40:24 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "minishell.h"

static int	is_n_flag(char *arg)
{
	int	j;

	if (arg[0] != '-')
		return (0);
	j = 1;
	if (arg[j] != 'n')
		return (0);
	while (arg[j] == 'n')
		j++;
	return (arg[j] == '\0');
}

static int	builtin_echo(t_shell *shell, char **args)
{
	int	i;
	int	nl;

	(void)shell;
	nl = 1;
	i = 1;
	while (args[i] && is_n_flag(args[i]))
	{
		nl = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (nl)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

static int	builtin_pwd(t_shell *shell, char **args)
{
	char	cwd[4096];

	(void)shell;
	(void)args;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	return (0);
}

static int	builtin_env(t_shell *shell, char **args)
{
	int	i;

	(void)args;
	i = 0;
	while (shell->envp && shell->envp[i])
	{
		if (ft_strchr(shell->envp[i], '='))
			ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

t_builtin_fn	get_builtin(char *name)
{
	if (!name)
		return (NULL);
	if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo);
	if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd);
	if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd);
	if (ft_strcmp(name, "export") == 0)
		return (builtin_export);
	if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset);
	if (ft_strcmp(name, "env") == 0)
		return (builtin_env);
	if (ft_strcmp(name, "exit") == 0)
		return (builtin_exit);
	return (NULL);
}
