/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 21:43:54 by marvin            #+#    #+#             */
/*   Updated: 2026/06/06 21:43:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redirs **redirs)
{
	int	i;

	i = 0;
	while (redirs && redirs[i])
	{
		if (redirs[i]->delimiter)
			free(redirs[i]->delimiter);
		if (redirs[i]->hd)
			free(redirs[i]->hd);
		free(redirs[i]);
		i++;
	}
	if (redirs)
		free(redirs);
}

void	free_ast(t_ast *ast)
{
	if (ast)
	{
		if (ast->ast_type == AST_PIPE)
		{
			if (ast->left)
				free_ast(ast->left);
			if (ast->right)
				free_ast(ast->right);
		}
		if (ast->ast_type == AST_CMD)
		{
			if (ast->args)
				free(ast->args);
			if (ast->redirs)
				free_redirs(ast->redirs);
		}
		free(ast);
	}
}

void	cleanup_ast(t_shell *shell)
{
	if (shell->ast)
		free_ast(shell->ast);
	shell->ast = NULL;
}
