/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 18:05:50 by danimend         #+#    #+#             */
/*   Updated: 2026/06/29 12:00:00 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "minishell.h"

static int	name_matches(char *entry, char *name)
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

static int	env_set_entry(t_shell *shell, char *entry)
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

static int	env_declare(t_shell *shell, char *name)
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

static int	env_unset(t_shell *shell, char *name)
{
	char	**new;
	int		count;
	int		i;
	int		j;

	count = ptrptr_count((void **)shell->envp);
	new = malloc(sizeof(char *) * (count + 1));
	if (!new)
		return (1);
	i = 0;
	j = 0;
	while (shell->envp[i])
	{
		if (name_matches(shell->envp[i], name))
			free(shell->envp[i]);
		else
			new[j++] = shell->envp[i];
		i++;
	}
	new[j] = NULL;
	free(shell->envp);
	shell->envp = new;
	return (0);
}

static int	set_kv(t_shell *shell, char *key, char *val)
{
	char	*tmp;
	char	*entry;
	int		ret;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (1);
	entry = ft_strjoin(tmp, val);
	free(tmp);
	if (!entry)
		return (1);
	ret = env_set_entry(shell, entry);
	free(entry);
	return (ret);
}

static int	is_valid_identifier(char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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

static int	is_numeric(char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (ft_issign(s[i]))
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

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

static int	export_print(char **envp)
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

static int	builtin_export(t_shell *shell, char **args)
{
	int	i;
	int	status;

	if (!args[1])
		return (export_print(shell->envp));
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			status = 1;
		}
		else if (ft_strchr(args[i], '='))
		{
			if (env_set_entry(shell, args[i]))
				status = 1;
		}
		else
		{
			if (env_declare(shell, args[i]))
				status = 1;
		}
		i++;
	}
	return (status);
}

static int	builtin_unset(t_shell *shell, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		env_unset(shell, args[i]);
		i++;
	}
	return (0);
}

static int	builtin_cd(t_shell *shell, char **args)
{
	char	*target;
	char	old[4096];
	char	new[4096];

	target = args[1];
	if (!target)
	{
		target = envp_value("HOME", NULL, shell->envp);
		if (!target)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (1);
		}
	}
	if (!getcwd(old, sizeof(old)))
	{
		perror("minishell: cd");
		return (1);
	}
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (!getcwd(new, sizeof(new)))
	{
		perror("minishell: cd");
		return (1);
	}
	set_kv(shell, "OLDPWD", old);
	set_kv(shell, "PWD", new);
	return (0);
}

static int	builtin_exit(t_shell *shell, char **args)
{
	int	status;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!args[1])
		cleanup_and_exit(shell, NULL, -1, shell->last_status);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		cleanup_and_exit(shell, NULL, -1, 255);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	status = (unsigned char)ft_atoi(args[1]);
	cleanup_and_exit(shell, NULL, -1, status);
	return (status);
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
