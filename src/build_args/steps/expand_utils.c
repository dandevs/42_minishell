/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 00:47:55 by marvin            #+#    #+#             */
/*   Updated: 2026/06/25 00:47:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_expand_string2(char **str, int i, char *var_new_val)
{
	char	*expanded;

	expanded = str_replace(*str, *str + i, var_new_val);
	free(var_new_val);
	if (!expanded)
		return (-1);
	free(*str);
	*str = expanded;
	return (1);
}

int	do_expand_string(char **str, int i, char *last_status, char **envp)
{
	char	*var_og_val;
	char	*var_new_val;
	int		len;

	len = 1;
	while ((*str)[i + len] && (ft_isalnum((*str)[i + len]) || (*str)[i + len] == '_'))
		len++;
	if (len == 1  && (*str)[i + len] == '?')
		len++;
	var_og_val = malloc(sizeof(char) * (len + 1));
	if (!var_og_val)
		return (-1);
	ft_strlcpy(var_og_val, *str + i , len + 1);
	var_new_val = envp_value(var_og_val + 1, last_status, envp);
	len = ft_strlen(var_new_val);
	var_new_val = str_replace(*str + i, var_og_val, var_new_val);
	free(var_og_val);
	if (!var_new_val)
		return (-1);
	if (do_expand_string2(str, i, var_new_val))
		return (len);
	return (-1);
}

int	check_expand_string(char **str, int hd, char *last_status, char **envp)
{
	t_mode	mode;
	int		i;
	int		str_len;

	i = 0;
	mode = NORMAL;
	if (!*str)
		return (1);
	while ((*str)[i])
	{
		mode_change(&mode, *str, i);
		if ((mode == NORMAL || mode == DOUBLEQ || hd == 1) && (*str)[i] == '$')
		{
			str_len = do_expand_string(str, i, last_status, envp);
			if (str_len == -1)
				return (0);
			i += str_len;
		}
		else
			i++;
	}
	return (1);
}
