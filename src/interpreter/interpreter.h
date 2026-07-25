/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:12:33 by danimend          #+#    #+#             */
/*   Updated: 2026/07/25 22:09:42 by marvin           ###   ########.fr       */
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

int				run_builtin_parent(t_interpreter_context *ctx, t_ast *cmd,
					t_builtin_fn fn);
void			run_cmd_child(t_ast *cmd, int fd_read, int fd_write,
					t_interpreter_context *ctx);

#endif
