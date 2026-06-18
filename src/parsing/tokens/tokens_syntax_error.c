/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_validity.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 13:57:45 by marvin            #+#    #+#             */
/*   Updated: 2026/06/05 13:57:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_syntax_error(t_tokens *iterate, char **error)
{
	t_tokens	*tmp;

	if (iterate->token == PIPE)
	{
		tmp = iterate->next;
		while (tmp && (*error) == NULL)
		{
			if (tmp->token == WORD)
				return (0);
			if (tmp->token == PIPE)
				*error = iterate->lexeme;
			tmp = tmp->next;
		}
		*error = iterate->lexeme;
	}
	else
		return (0);
	return (1);
}

int	quote_syntax_error(t_tokens *iterate, char **error)
{
	t_mode	mode;
	int		i;

	mode = NORMAL;
	i = 0;
	while (iterate->lexeme[i])
	{
		if ((iterate->lexeme[i] == '\"' && mode == DOUBLEQ) ||
		(iterate->lexeme[i] == '\'' && mode == SINGLEQ))
			mode = NORMAL;
		else if (iterate->lexeme[i] == '\'' && mode == NORMAL)
			mode = SINGLEQ;
		else if (iterate->lexeme[i] == '\"' && mode == NORMAL)
			mode = DOUBLEQ;
		i++;
	}
	if (mode == SINGLEQ)
		*error = "\'";
	else if (mode == DOUBLEQ)
		*error = "\"";
	else
		return (0);
	return (1);
}

int	basic_syntax_error(t_tokens *tokens, t_tokens *iterate, char **error)
{
	if (iterate->token == PIPE && (iterate == tokens || !iterate->next))
		*error = iterate->lexeme;
	else if ((iterate->token == LESSER || iterate->token == DLESSER)
		&& (!iterate->next || iterate->next->token != WORD))
		*error = iterate->lexeme;
	else if ((iterate->token == GREATER || iterate->token == DGREATER)
		&& (!iterate->next || iterate->next->token != WORD))
		*error = iterate->lexeme;
	else
		return (0);
	return (1);
}

int	tokens_syntax_error(t_tokens *tokens)
{
	char		*error;
	t_tokens	*iterate;

	iterate = tokens;
	error = NULL;
	while (iterate && !error)
	{
		if (basic_syntax_error(tokens, iterate, &error))
			break ;
		if (quote_syntax_error(iterate, &error))
			break ;
		if (pipe_syntax_error(iterate, &error))
			break ;
		iterate = iterate->next;
	}
	if (error)
	{
		ft_putstr_fd("Syntax error near token `", 2);
		ft_putstr_fd(error, 2);
		ft_putstr_fd("`\n", 2);
		return (1);
	}
	return (0);
}
// quotes devrait peut etre etre detectees des tokens_value. (donc renvoyer null?).
// AUSSI : SYNTAX error doit indiquer le token suivant qui n est pas suppose etre la
