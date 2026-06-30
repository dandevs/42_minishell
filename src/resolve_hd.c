/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_hd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 01:58:31 by mat               #+#    #+#             */
/*   Updated: 2026/07/01 02:10:44 by marvin           ###   ########.fr       */
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
	char	*line;
	char	*tmp;

	redirs->hd = ft_strdup("");
	if (!redirs->hd)
		return (0);
	line = readline("> ");
	while (line && ft_strcmp(line, redirs->delimiter) != 0)
	{
		tmp = ft_strjoin(redirs->hd, line);
		free(line);
		if (!tmp)
			return (0);
		free(redirs->hd);
		redirs->hd = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!redirs->hd)
			return (0);
		line = readline("> ");
	}
	if (!line)
		ft_putstr_fd("minishell: warning: heredoc delimited by EOF\n", 2);
	else
		free(line);
	return (1);
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
