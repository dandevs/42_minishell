/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:12:33 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 20:14:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "minishell.h"
# include <sys/wait.h>
# include <fcntl.h>

//char		**build_argv(t_tokens *start, t_tokens *end);
typedef int	(*t_builtin_fn)(t_shell *shell, char **args);

t_builtin_fn	get_builtin(char *name);
int				apply_one_redir(t_tokens *op, t_tokens *file);
int				run_builtin_parent(t_interpreter_context *ctx, t_ast *cmd,
					t_builtin_fn fn);
char			*resolve_path(char *name, char **envp);
void			run_cmd(t_ast *cmd, int fd_read, int fd_write,
					t_interpreter_context *ctx);
int				has_slash(char *name);
char			**get_path_dirs(char *name, char **envp);
char			*search_path_dirs(char **dirs, char *name);
void			setup_child_fds(int fd_read, int fd_write);
void			close_inherited_fds(void);
void			exec_child(t_ast *cmd, t_interpreter_context *ctx);

#endif
