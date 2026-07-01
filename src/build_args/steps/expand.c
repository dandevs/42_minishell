/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 22:08:18 by mat               #+#    #+#             */
/*   Updated: 2026/06/25 00:52:21 by marvin           ###   ########.fr       */
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

int	expand_tokens(t_tokens *start, t_tokens *end, char *last_status, char **envp)
{
	int	redir_file;

	redir_file = 0;
	while (start != end && start)
	{
		if (start->token != WORD || redir_file == 1)
		{
			if (redir_file == 1)
				redir_file = 0;
			else if (start->token != PIPE && redir_file == 0)
				redir_file = 1;
			start = start->next;
			continue ;
		}
		if (!check_expand_string(&(start->lexeme), 0, last_status, envp))
			return (0);
		start = start->next;
	}
	return (1);
}

int	expand_redirs(t_redirs	**redirs, char *last_status, char **envp)
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
		if (expand_hd && !check_expand_string(&(redirs[i]->hd), 1, last_status, envp))
			return (0);
		if (redirs[i]->file && !check_expand_string(&(redirs[i]->file->lexeme), 0, last_status, envp))
			return (0);
		i++;
	}
	return (1);
}

int	expand(t_ast *ast, int last_stat, char **envp)
{
	char	*last_status;

	last_status = ft_itoa(last_stat);
	if (!last_status)
		return (0);
	if (!expand_redirs(ast->redirs, last_status, envp))
		return (free(last_status), 0);
	if (!expand_tokens(ast->start, ast->end, last_status, envp))
		return (free(last_status), 0);
	free(last_status);
	return (1);
}

