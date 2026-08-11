/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 22:08:12 by marvin            #+#    #+#             */
/*   Updated: 2026/07/25 22:08:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_H
# define RUN_H

# include "interpreter.h"
# include <sys/types.h>
# include <sys/stat.h>

int				apply_one_redir(t_tokens *op, t_tokens *file);
int				apply_one_heredoc(char *hd);
int				process_redirs(t_ast *cmd, t_redir_cb cb, t_heredoc_cb hd_cb);

int				has_slash(char *name);
char			**get_path_dirs(char *name, char **envp);
char			*search_path_dirs(char **dirs, char *name);

void			setup_child_fds(int fd_read, int fd_write);
void			close_inherited_fds(t_interpreter_context *ctx,
					int fd_read, int fd_write);
void			exec_child(t_ast *cmd, t_interpreter_context *ctx);

#endif
