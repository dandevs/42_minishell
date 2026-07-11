/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 12:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/06/27 06:56:51 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

# define BUILTINS_COUNT 7

typedef int	(*t_builtin_fn)(t_shell *shell, char **args);

t_builtin_fn	get_builtin(char *name);
int			name_matches(char *entry, char *name);
int			env_set_entry(t_shell *shell, char *entry);
int			env_declare(t_shell *shell, char *name);
int			env_unset(t_shell *shell, char *name);
int			set_kv(t_shell *shell, char *key, char *val);
int			is_valid_identifier(char *s);
int			export_print(char **envp);
int			builtin_export(t_shell *shell, char **args);
int			builtin_unset(t_shell *shell, char **args);
int			builtin_cd(t_shell *shell, char **args);
int			builtin_exit(t_shell *shell, char **args);

#endif
