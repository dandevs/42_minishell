/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_read.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 14:34:02 by danimend          #+#    #+#             */
/*   Updated: 2026/08/24 14:36:35 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hd_append_char(char **out, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(*out, buf);
	if (!tmp)
		return (free(*out), 0);
	free(*out);
	*out = tmp;
	return (1);
}

/* Reads one byte from stdin. Returns 1 = byte, 0 = EOF, -1 = error/SIGINT. */
static int	hd_read_byte(char *c)
{
	ssize_t	n;

	n = read(STDIN_FILENO, c, 1);
	if (n > 0)
		return (1);
	if (n < 0 && g_signal == SIGINT)
		return (-1);
	return (0);
}

static int	hd_read_line(char **out)
{
	char	c;
	int		r;

	*out = ft_strdup("");
	if (!*out)
		return (0);
	while (1)
	{
		r = hd_read_byte(&c);
		if (r < 0)
			return (free(*out), -1);
		if (r == 0)
		{
			if (**out)
				return (1);
			return (free(*out), 0);
		}
		if (c == '\n')
			return (1);
		if (!hd_append_char(out, c))
			return (0);
	}
}

static int	hd_append_line(t_redirs *redirs, char *line)
{
	char	*tmp;

	tmp = ft_strjoin(redirs->hd, line);
	free(line);
	if (!tmp)
		return (0);
	free(redirs->hd);
	redirs->hd = ft_strjoin(tmp, "\n");
	free(tmp);
	if (!redirs->hd)
		return (0);
	return (1);
}

int	hd_collect_body(t_redirs *redirs)
{
	char	*line;
	int		r;

	r = 1;
	while (r == 1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "> ", 2);
		r = hd_read_line(&line);
		if (r < 0)
			write(STDOUT_FILENO, "\n", 1);
		else if (r == 0)
			ft_putstr_fd("minishell: warning: heredoc delimited by EOF\n", 2);
		else if (ft_strcmp(line, redirs->delimiter) == 0)
		{
			free(line);
			break ;
		}
		else if (!hd_append_line(redirs, line))
			r = -1;
	}
	return (r);
}
