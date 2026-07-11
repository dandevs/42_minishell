/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:40:10 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 17:40:12 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "minishell.h"

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

int	builtin_exit(t_shell *shell, char **args)
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
