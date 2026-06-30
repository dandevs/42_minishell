/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 02:39:17 by marvin            #+#    #+#             */
/*   Updated: 2026/04/23 02:39:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

int	main(int agc, char **agv, char **envp)
{
	t_shell	shell;

	setup(agc, agv, envp, &shell);
	print_envp(shell.envp);
	while (1)
	{
		shell.line = get_line(&shell);
		if (!shell.line)
			cleanup_and_exit(&shell, "exit\n", STDOUT_FILENO, 0);
		if (str_empty(shell.line))
			continue ;
		if (!parse(&shell) || !resolve_hd_recursive(shell.ast)
			|| !build_args_recursive(shell.ast, shell.last_status, shell.envp))
		{
			cleanup_loop(&shell);
			continue ;
		}
		print_redirs(shell.ast);
		print_tokens(&shell);
		shell.last_status = interpret(shell.ast).exit_status;
		cleanup_loop(&shell);
	}
}
