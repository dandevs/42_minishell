/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:39:17 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 18:27:43 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "interpreter.h"
#include <fcntl.h>

static int	is_redir(t_token t)
{
	return (t == GREATER || t == DGREATER || t == LESSER || t == DLESSER);
}

static void	skip_redir_token_and_word(t_tokens **head)
{
	*head = (*head)->next;
	if (*head)
		*head = (*head)->next;
}

static int	count_argv(t_tokens *start, t_tokens *end)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (is_redir(start->token))
		{
			skip_redir_token_and_word(&start);
			continue ;
		}
		if (start->token == WORD)
			count++;
		start = start->next;
	}
	return (count);
}

char	**build_argv(t_tokens *start, t_tokens *end)
{
	char	**argv;
	int		i;

	argv = malloc(sizeof(char *) * (count_argv(start, end) + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (start && start != end)
	{
		if (is_redir(start->token))
		{
			skip_redir_token_and_word(&start);
			continue ;
		}
		if (start->token == WORD)
			argv[i++] = start->lexeme;
		start = start->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	apply_one_redir(t_tokens *op, t_tokens *file)
{
	int	fd;
	int	target;

	if (op->token == GREATER)
		fd = open(file->lexeme, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (op->token == DGREATER)
		fd = open(file->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (op->token == LESSER)
		fd = open(file->lexeme, O_RDONLY);
	else
		return (1);
	if (fd < 0)
		return (perror(file->lexeme), 0);
	if (op->token == LESSER)
		target = STDIN_FILENO;
	else
		target = STDOUT_FILENO;
	dup2(fd, target);
	close(fd);
	return (1);
}
