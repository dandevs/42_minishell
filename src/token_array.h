/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_array.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:28:50 by danimend          #+#    #+#             */
/*   Updated: 2026/06/18 10:44:48 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_ARRAY_H
# define TOKEN_ARRAY_H

# include "token.h"

int	token_array_get_cmd(char **result, t_token_array *arr);
int	token_array_get_args(char ***args, t_token_array *arr);

#endif
