/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 22:08:18 by mat               #+#    #+#             */
/*   Updated: 2026/06/23 01:33:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*int	expand_var(char *var, char **var_new_val, char **var_og_val, char **envp)
{
	int		i;

	i = 1;
	while (var[i] && (ft_isalnum(var[i]) || var[i] == '_'))
		i++;
	*var_og_val = malloc(sizeof(char) * (i + 1));
	if (!*var_og_val)
		return (0);
	ft_strlcpy(*var_og_val, var, i + 1);
	*var_new_val = envp_value((*var_og_val) + 1, envp);
	return (1);
}*/

int	do_expand_string(char **str, int i, char **envp)
{
	char	*var_og_val;
	char	*var_new_val;
	char	*expanded;
	int		len;

	len = 1;
	while ((*str)[i + len] && (ft_isalnum((*str)[i + len]) || (*str)[i + len] == '_'))
		len++;
	var_og_val = malloc(sizeof(char) * (len + 1));
	if (!var_og_val)
		return (-1);
	ft_strlcpy(var_og_val, *str + i , len + 1);
	var_new_val = envp_value(var_og_val + 1, envp);
	len = ft_strlen(var_new_val);
	var_new_val = str_replace(*str + i, var_og_val, var_new_val);
	free(var_og_val);
	if (!var_new_val)
		return (-1);
	expanded = str_replace(*str, *str + i, var_new_val);
	free(var_new_val);
	if (!expanded)
		return (-1);
	free(*str);
	*str = expanded;
	return (len);
}

int	check_expand_string(char **str, int hd, char **envp)
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
			str_len = do_expand_string(str, i, envp);
			if (str_len == -1)
				return (0);
			i += str_len;
		}
		else
			i++;
	}
	return (1);
}

int	expand_node_tokens(t_tokens *start, t_tokens *end, char **envp)
{
	while (start != end && start)
	{
		if (start->token != WORD)
		{
			start = start->next;
			continue ;
		}
		if (!check_expand_string(&(start->lexeme), 0, envp))
			return (0);
		start = start->next;
	}
	return (1);
}

int	expand_node_redirs(t_redirs	**redirs, char **envp)
{
	int		i;
	int		j;
	int		expand_hd;
	t_mode	mode;

	i = 0;
	while (redirs[i])
	{
		j = 0;
		expand_hd = 1;
		mode = NORMAL;
		while (redirs[i]->delimiter && redirs[i]->delimiter[j])
		{
			if (mode_change(&mode, redirs[i]->delimiter, j))
				expand_hd = 0;
			j++;
		}
		if (expand_hd && !check_expand_string(&(redirs[i]->hd), 1, envp))
			return (0);
		if (redirs[i]->file && !check_expand_string(&(redirs[i]->file->lexeme), 0, envp))
			return (0);
		i++;
	}
	return (1);
}

int	expand_node(t_ast *ast, char **envp)
{
	if (!expand_node_tokens(ast->start, ast->end, envp))
		return (0);
	if (!expand_node_redirs(ast->redirs, envp))
		return (0);
	return (1);
}

