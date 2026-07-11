/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:12:33 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 16:51:19 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "minishell.h"
# include "builtins.h"

char		**build_argv(t_tokens *start, t_tokens *end);
int			apply_one_redir(t_tokens *op, t_tokens *file);
int			run_builtin_parent(t_interpreter_context *ctx, t_ast *cmd,
				t_builtin_fn fn);
char		*resolve_path(char *name, char **envp);
void		run_cmd(t_ast *cmd, int fd_read, int fd_write,
				t_interpreter_context *ctx);
int			has_slash(char *name);
char		**get_path_dirs(char *name, char **envp);
char		*search_path_dirs(char **dirs, char *name);
void		setup_child_fds(int fd_read, int fd_write);
void		close_inherited_fds(void);
void		exec_child(t_ast *cmd, t_interpreter_context *ctx);

#endif
