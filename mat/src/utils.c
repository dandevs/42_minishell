/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 22:19:19 by mat               #+#    #+#             */
/*   Updated: 2026/06/15 22:22:05 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int mode_change(t_mode *mode, char *word, int i)
{
    if (word[i] == '"' && *mode == NORMAL)
        *mode = DOUBLEQ;
    else if (word[i] == '\'' && *mode == NORMAL)
        *mode = SINGLEQ;
    else if (word[i] == '\'' && *mode == SINGLEQ)
        *mode = NORMAL;
    else if (word[i] == '"' && *mode == DOUBLEQ)
        *mode = NORMAL;
    else
        return (0);
    return (1);
}
