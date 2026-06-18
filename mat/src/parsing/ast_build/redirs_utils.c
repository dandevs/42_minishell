/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_parsing_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 22:22:47 by marvin            #+#    #+#             */
/*   Updated: 2026/06/07 22:22:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_redirs(t_tokens *start, t_tokens *end)
{
	int	i;

	i = 0;
	while (start && start != end)
	{
		if (start->token != WORD)
			i++;
		start = start->next;
	}
	return (i);
}

int	get_redirs_fail(t_ast **ast, t_redirs **redirs, int i)
{
	if (redirs[i])
		free(redirs[i]);
	while (--i && i >= 0)
		free(redirs[i]);
	free(redirs);
	(*ast)->redirs = NULL;
	return (0);
}

int	get_redirs_file(t_redirs *redirs, t_tokens *start)
{
	t_tokens	*tmp;

	tmp = start;
	while (tmp->token != WORD)
		tmp = tmp->next;
	if (!tmp)
		return (0);
	redirs->file = tmp;
	return (1);
}

int	get_redirs(t_ast **ast, t_tokens *start, t_tokens *end)
{
	int	count;
	int	i;

	if (!(*ast)->redirs)
		return (0);
	count = count_redirs(start, end);
	i = 0;
	while (start && start != end && i < count)
	{
		if (start->token != WORD && start->token != PIPE)
		{
			(*ast)->redirs[i] = malloc(sizeof(t_redirs));
			if (!(*ast)->redirs[i])
				return (get_redirs_fail(ast, (*ast)->redirs, i));
			(*ast)->redirs[i]->tokens = start;
			if (!get_redirs_file((*ast)->redirs[i], start))
				return (get_redirs_fail(ast, (*ast)->redirs, i));
			i++;
		}
		start = start->next;
	}
	return (1);
}

/*int	get_args(t_ast **ast, t_tokens *start, t_tokens *end)
{
	int	count;
	int	i;

	if (!(*ast)->args)
		return (0);
	count = count_args(start, end);
	i = 0;
	while (start && start != end && i < count)
	{
		if (start->token == WORD)
		{
			(*ast)->args[i] = start->lexeme;
			i++;
		}
		start = start->next;
	}
	return (1);
}*/
