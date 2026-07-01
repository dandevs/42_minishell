/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 15:00:31 by marvin            #+#    #+#             */
/*   Updated: 2026/06/07 15:00:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ast_initialize(t_ast **ast, t_tokens *start, t_tokens *end, t_tokens *binary)
{
	*ast = calloc(1, sizeof(t_ast));
	if (!*ast)
		return (0);
	if (binary)
		(*ast)->ast_type = AST_PIPE;
	else
		(*ast)->ast_type = AST_CMD;
	(*ast)->start = start;
	(*ast)->end = end;
	return (1);
}

t_tokens	*tokens_binary_get(t_tokens *start, t_tokens *end)
{
	while (start && start != end)
	{
		if (start->token == PIPE)
			return (start);
		start = start->next;
	}
	return (NULL);
}

int	ast_build_recursive(t_ast **ast, t_tokens *start, t_tokens *end)
{
	t_tokens	*binary;

	if (!start)
		return (1);
	binary = tokens_binary_get(start, end);
	if (!ast_initialize(ast, start, end, binary))
		return (0);
	if ((*ast)->ast_type == AST_PIPE)
	{
		if (ast_build_recursive(&((*ast)->left), start, binary)
			&& ast_build_recursive(&((*ast)->right), binary->next, end))
			return (1);
	}
	else
	{
		(*ast)->redirs = calloc((count_redirs(start, end) + 1), sizeof(t_redirs *));
		if (get_redirs(ast, start, end))
			return (1);
	}
	free_ast(*ast);
	*ast = NULL;
	return (0);
}
