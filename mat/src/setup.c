/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 19:12:25 by marvin            #+#    #+#             */
/*   Updated: 2026/04/23 19:12:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// exit code 3 :
// Invalid argument: An incorrect or missing argument was provided
// to the script

//envp : char ** recree a chaque set / unset / export ->oui ?

/*t_list	*lst_envp(char **envp)
{
	t_list	*lst;
	t_list	*node;
	char	*content;
	int		i;

	i = 0;
	lst = NULL;
	while (envp[i])
	{
		content = ft_strdup(envp[i]);
		if (!content)
		{
			ft_lstclear(&lst, free);
			return (NULL);
		}
		node = ft_lstnew(content);
		if (!node)
		{
			ft_lstclear(&lst, free);
			free(content);
			return (NULL);
		}
		ft_lstadd_back(&lst, node);
		i++;
	}
	return (lst);
}*/

char	**setup_envp(char **envp)
{
	int	i;
	char	**dup;

	i = 0;
	dup = malloc(sizeof(char *) * (ptrptr_count((void **)envp) + 1));
	if (!dup)
		return (0);
	while (envp && envp[i])
	{
		dup[i] = ft_strdup(envp[i]);
		if (!dup[i])
		{
			ptrptr_free((void **)dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}


void	setup(int agc, char **agv, char **envp, t_shell *shell)
{
	(void)agc;
	(void)agv;
	if (agc != 1 || !envp || !*envp)
		cleanup_and_exit(shell, "incorrect args", 2, 3);
	shell->last_line = NULL;
	shell->ast = NULL;
	shell->envp = setup_envp(envp);
	shell->last_status = 0;
	shell->line = NULL; //malloc = exit;	pas normal ?			3 lignes = setup?
	shell->tokens = NULL;
	if (!shell->envp || !shell->envp[0])
		cleanup_and_exit(shell, "couldnt create envp dup", 2, 71);	// exit malloc*/
	setup_signals(shell); //erreur de signal exi;
}
