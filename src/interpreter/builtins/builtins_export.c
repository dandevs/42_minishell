/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:40:13 by danimend          #+#    #+#             */
/*   Updated: 2026/07/26 02:57:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "minishell.h"

static int	process_export_arg(t_shell *shell, char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	if (ft_strchr(arg, '='))
		return (env_set_entry(shell, arg));
	return (env_declare(shell, arg));
}

int	builtin_export(t_shell *shell, char **args)
{
	int	i;
	int	status;

	if (!args[1])
		return (export_print(shell->envp));
	status = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		if (process_export_arg(shell, args[i]))
			status = EXIT_FAILURE;
		i++;
	}
	return (status);
}
