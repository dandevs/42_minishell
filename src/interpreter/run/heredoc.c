/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:39:17 by danimend          #+#    #+#             */
/*   Updated: 2026/07/25 22:14:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

#define HD_TMP_ATTEMPTS 8

static int	write_all(int fd, const char *buf, size_t len)
{
	ssize_t	written;

	while (len > 0)
	{
		written = write(fd, buf, len);
		if (written <= 0)
			return (0);
		buf += written;
		len -= written;
	}
	return (1);
}

static char	*make_tmp_path(void)
{
	unsigned char	raw[6];
	char			name[7];
	char			*path;
	int				fd;
	int				i;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (NULL);
	if (read(fd, raw, 6) != 6)
	{
		close(fd);
		return (NULL);
	}
	close(fd);
	i = 0;
	while (i < 6)
	{
		name[i] = 'a' + raw[i] % 26;
		i++;
	}
	name[6] = '\0';
	path = ft_strjoin("/tmp/.minishell_hd_", name);
	return (path);
}

static int	open_tmp_exclusive(char **path_out)
{
	int	fd;
	int	attempt;

	fd = -1;
	*path_out = NULL;
	attempt = 0;
	while (fd < 0 && attempt++ < HD_TMP_ATTEMPTS)
	{
		free(*path_out);
		*path_out = make_tmp_path();
		if (!*path_out)
			return (-1);
		fd = open(*path_out, O_WRONLY | O_CREAT | O_EXCL, 0600);
	}
	return (fd);
}

int	apply_one_heredoc(char *hd)
{
	char	*path;
	int		fd;

	fd = open_tmp_exclusive(&path);
	if (fd < 0)
		return (free(path), perror("minishell: heredoc"), 0);
	if (!write_all(fd, hd, ft_strlen(hd)))
	{
		close(fd);
		unlink(path);
		return (free(path), perror("minishell: heredoc"), 0);
	}
	close(fd);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (free(path), perror("minishell: heredoc"), 0);
	unlink(path);
	free(path);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}
