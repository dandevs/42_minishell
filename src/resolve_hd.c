/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_hd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 01:58:31 by mat               #+#    #+#             */
/*   Updated: 2026/07/02 14:44:54 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_no_quotes(char *word)
{
	int		i;
	int		count;
	t_mode	mode;

	i = 0;
	count = 0;
	mode = NORMAL;
	while (word[i])
	{
		if (!mode_change(&mode, word, i))
			count++;
		i++;
	}
	return (count);
}

int	get_hd_delimiter(t_redirs *redirs)
{
	int		i;
	int		j;
	t_mode	mode;

	i = count_no_quotes(redirs->file->lexeme);
	redirs->delimiter = malloc(sizeof(char) * (i + 1));
	if (!redirs->delimiter)
		return (0);
	i = 0;
	j = 0;
	mode = NORMAL;
	while (redirs->file->lexeme[i])
	{
		if (!mode_change(&mode, redirs->file->lexeme, i))
			redirs->delimiter[j++] = redirs->file->lexeme[i];
		i++;
	}
	redirs->delimiter[j] = '\0';
	return (1);
}

int	get_hd_body(t_redirs *redirs)
{
	int	r;

	redirs->hd = ft_strdup("");
	if (!redirs->hd)
		return (0);
	setup_signals_hd();
	r = hd_collect_body(redirs);
	if (r < 0)
	{
		free(redirs->hd);
		redirs->hd = NULL;
	}
	restore_signals();
	return (r >= 0);
}

int	resolve_hd_recursive(t_ast *ast)
{
	int	i;

	i = 0;
	if (ast->ast_type == AST_PIPE)
	{
		if (!resolve_hd_recursive(ast->left))
			return (0);
		if (!resolve_hd_recursive(ast->right))
			return (0);
		return (1);
	}
	while (ast->redirs[i])
	{
		if (ast->redirs[i]->tokens->token == DLESSER)
		{
			if (!get_hd_delimiter(ast->redirs[i]))
				return (0);
			if (!get_hd_body(ast->redirs[i]))
				return (0);
			ast->redirs[i]->file = NULL;
		}
		i++;
	}
	return (1);
}
