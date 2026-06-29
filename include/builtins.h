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

#endif
