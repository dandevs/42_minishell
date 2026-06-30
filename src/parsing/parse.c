/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 00:51:12 by mat               #+#    #+#             */
/*   Updated: 2026/06/28 01:49:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse(t_shell *shell)
{
	if (!tokens_get(shell, shell->line) || !shell->tokens
		|| tokens_syntax_error(shell->tokens))
	{
		if (!shell->tokens)
		{
			ft_putstr_fd("minishell: ", 2);
			perror("malloc");
		}
		return (0);
	}
	if (!ast_build_recursive(&(shell->ast), shell->tokens, NULL) || !shell->ast)
	{
		if (!shell->ast)
		{
			ft_putstr_fd("minishell: ", 2);
			perror("malloc");
		}
		return (0);
	}
	return (1);
}
