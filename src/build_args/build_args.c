/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 01:33:10 by mat               #+#    #+#             */
/*   Updated: 2026/06/23 22:19:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ambiguous_redirect(t_redirs **redirs)
{
	int		i;
	int		j;
	t_mode	mode;

	i = 0;
	while (redirs[i])
	{
		mode = NORMAL;
		j = 0;
		if (redirs[i]->file && !redirs[i]->file->lexeme[j])
			return (ft_printf("ambiguous redirect\n"), 1);
		while (redirs[i]->file && redirs[i]->file->lexeme[j])
		{
			mode_change(&mode, redirs[i]->file->lexeme, j);
			if (mode == NORMAL && ft_isspace(redirs[i]->file->lexeme[j]))
				return (ft_printf("ambiguous redirect\n"), 1);
			j++;
		}
		i++;
	}
	return (0);
}

int build_args_recursive(t_ast *ast, char **envp)
{
	t_list	*args_lst;

	if (ast->ast_type == AST_PIPE)
	{
		if (!build_args_recursive(ast->left, envp) || !build_args_recursive(ast->right, envp))
			return (0);
	}
	else
	{
		if (!expand(ast, envp) || ambiguous_redirect(ast->redirs))
			return (0);
		args_lst = NULL;
		if (!tokens_to_lst(&args_lst, ast->start, ast->end))
			return (0);
		if (!finalize_lst(&args_lst))
			return (0);
		if (!lst_to_args(args_lst, ast))
			return (0);
		print_args(ast);
	}
	return (1);
}
