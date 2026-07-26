/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 01:33:10 by mat               #+#    #+#             */
/*   Updated: 2026/07/26 04:39:58 by marvin           ###   ########.fr       */
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

int	remove_quotes_redirs(t_redirs **redirs)
{
	t_mode	mode;
	int		i;
	int		j;

	j = -1;
	while (redirs[++j])
	{
		if (redirs[j]->file == NULL)
			continue ;
		mode = NORMAL;
		i = 0;
		while (redirs[j]->file->lexeme[i])
		{
			if (mode_change(&mode, redirs[j]->file->lexeme, i))
				ft_memmove(redirs[j]->file->lexeme + i,
					redirs[j]->file->lexeme + i + 1,
					1 + ft_strlen(redirs[j]->file->lexeme + i));
			else
				i++;
		}
	}
	return (1);
}

int	build_args_recursive(t_ast *ast, int last_status, char **envp)
{
	t_list	*args_lst;

	if (ast->ast_type == AST_PIPE)
	{
		if (!build_args_recursive(ast->left, last_status, envp)
			|| !build_args_recursive(ast->right, last_status, envp))
			return (0);
	}
	else
	{
		if (!expand(ast, last_status, envp) || ambiguous_redirect(ast->redirs))
			return (0);
		remove_quotes_redirs(ast->redirs);
		args_lst = NULL;
		if (!tokens_to_lst(&args_lst, ast->start, ast->end))
			return (0);
		if (!finalize_lst(&args_lst))
			return (0);
		if (!lst_to_args(args_lst, ast))
			return (0);
	}
	return (1);
}
