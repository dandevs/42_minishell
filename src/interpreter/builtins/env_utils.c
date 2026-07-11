/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:40:28 by danimend          #+#    #+#             */
/*   Updated: 2026/07/11 17:40:29 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "minishell.h"

int	env_unset(t_shell *shell, char *name)
{
	char	**new;
	int		count;
	int		i;
	int		j;

	count = ptrptr_count((void **)shell->envp);
	new = malloc(sizeof(char *) * (count + 1));
	if (!new)
		return (1);
	i = 0;
	j = 0;
	while (shell->envp[i])
	{
		if (name_matches(shell->envp[i], name))
			free(shell->envp[i]);
		else
			new[j++] = shell->envp[i];
		i++;
	}
	new[j] = NULL;
	free(shell->envp);
	shell->envp = new;
	return (0);
}

int	set_kv(t_shell *shell, char *key, char *val)
{
	char	*tmp;
	char	*entry;
	int		ret;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (1);
	entry = ft_strjoin(tmp, val);
	free(tmp);
	if (!entry)
		return (1);
	ret = env_set_entry(shell, entry);
	free(entry);
	return (ret);
}

int	is_valid_identifier(char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
