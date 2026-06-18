/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 16:00:31 by marvin            #+#    #+#             */
/*   Updated: 2026/06/04 16:00:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokens_lexeme_value2(t_tokens *tokens, char *line, int i)
{
	int		j;
	t_mode	mode;

	j = i;
	mode = NORMAL;
	while (line[j])
	{
		if (ft_isspace(line[j]) && mode == NORMAL)
			break ;
		else if ((line[j] == '\"' && mode == DOUBLEQ) || (line[j] == '\''
				&& mode == SINGLEQ))
			mode = NORMAL;
		else if (line[j] == '\'' && mode == NORMAL)
			mode = SINGLEQ;
		else if (line[j] == '\"' && mode == NORMAL)
			mode = DOUBLEQ;
		else if ((line[j] == '|' || line[j] == '>' || line[j] == '<')
			&& mode == NORMAL)
			break ;
		j++;
	}
	tokens->lexeme = ft_substr(line, i, (unsigned int)(j - i));
}

void	tokens_lexeme_value(t_tokens *tokens, t_token token_val,
	char *line, int i)
{
	if (token_val == DGREATER || token_val == DLESSER)
		tokens->lexeme = ft_substr(line, i, 2);
	else if (token_val == LESSER || token_val == GREATER || token_val == PIPE)
		tokens->lexeme = ft_substr(line, i, 1);
	else
		tokens_lexeme_value2(tokens, line, i);
}

void	tokens_token_value(t_tokens *tokens, char *line, int i)
{
	if (!line || !line[i])
		return ;
	else if (line[i] == '|')
		tokens->token = PIPE;
	else if (line[i] == '>' && line[i + 1] == '>')
		tokens->token = DGREATER;
	else if (line[i] == '>')
		tokens->token = GREATER;
	else if (line[i] == '<' && line[i + 1] == '<')
		tokens->token = DLESSER;
	else if (line[i] == '<')
		tokens->token = LESSER;
	else if (!ft_isspace(line[i]))
		tokens->token = WORD;
}

t_tokens	*tokens_values(t_tokens *last, char *line, int i)
{
	t_tokens	*tokens;

	tokens = calloc(1, sizeof(t_tokens));
	if (!tokens)
		return (NULL);
	tokens->litteral = NULL;
	tokens_token_value(tokens, line, i);
	tokens_lexeme_value(tokens, tokens->token, line, i);
	if (last)
		last->next = tokens;
	return (tokens);
}
