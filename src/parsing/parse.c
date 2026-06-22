/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 00:51:12 by mat               #+#    #+#             */
/*   Updated: 2026/06/22 21:50:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int parse(t_shell *shell)
{
	if (!tokens_get(shell, shell->line) || !shell->tokens || tokens_syntax_error(shell->tokens))
	{
		if (!shell->tokens)
			ft_printf("tokens failed to be allocated : if this problem persists, try restarting minishell by pressing Ctrl+D on an empty line.\n");
		return (0);
	}
	//print_tokens(shell);
	if (!ast_build_recursive(&(shell->ast), shell->tokens, NULL) || !shell->ast /*|| ast_invalid(shell.ast)*/)
	{
		if (!shell->ast)
			ft_printf("ast failed to be allocated : if this problem persists, try restarting minishell by pressing Ctrl+D on an empty line.\n");
		return (0);
	}
	//print_ast(shell->ast);
	return (1);
}
