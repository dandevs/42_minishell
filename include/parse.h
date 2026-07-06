/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 03:35:08 by marvin            #+#    #+#             */
/*   Updated: 2026/07/06 03:35:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			tokens_syntax_error(t_tokens *tokens);

int			tokens_get(t_shell *shell, char *line);
t_tokens	*tokens_values(t_tokens *last, char *line, int i);

int			ast_build_recursive(t_ast **ast, t_tokens *start, t_tokens *end);
int			get_args(t_ast **ast, t_tokens *start, t_tokens *end);
int			get_redirs(t_ast **ast, t_tokens *start, t_tokens *end);
int			count_redirs(t_tokens *start, t_tokens *end);
int			count_args(t_tokens *start, t_tokens *end);
