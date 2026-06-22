/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 22:08:18 by mat               #+#    #+#             */
/*   Updated: 2026/06/22 21:18:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_var(char *var, char **var_new_val, char **var_og_val, char **envp)
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
}

int	expand_lexeme(char **lex, int i, char **envp)
{
	char	*var_og_val;
	char	*var_new_val;
	char	*expanded;

	if (!expand_var(*lex + i, &var_new_val, &var_og_val, envp))
		return (-1);
	expanded = str_replace(*lex, var_og_val, var_new_val);
	free(var_og_val);
	if (!expanded)
		return (-1);
	free(*lex);
	*lex = expanded;
	return (ft_strlen(var_new_val));
}

int	expand_tokens(t_tokens *start, t_tokens *end, char **envp)
{
	int		i;
	int		j;
	t_mode	mode;

	while (start != end && start)
	{
		i = 0;
		mode = NORMAL;
		while (start->lexeme[i])
		{
			mode_change(&mode, start->lexeme, i);
			if ((mode == NORMAL || mode == DOUBLEQ) && start->lexeme[i] == '$')
			{
				j = expand_lexeme(&(start->lexeme), i, envp);
				if (j == -1)
					return (0);
				i += j;
			}
			else
				i++;
		}
		start = start->next;
	}
	return (1);
}

/*int	expand_redirs(t_redirs	**redirs, char **envp)
{
	int	i;

	i = 0;
	while ()
}*/

int	expand_node(t_ast *ast, char **envp)
{
	if (!expand_tokens(ast->start, ast->end, envp))
		return (0);
	//expand_redirs(ast->redirs);
	return (1);
}

