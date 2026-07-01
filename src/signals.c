/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 20:04:51 by marvin            #+#    #+#             */
/*   Updated: 2026/04/26 20:04:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_signal(t_shell *shell, int sig)
{
	if (g_signal == SIGINT)
	{
		shell->last_status = 128 + g_signal;
		g_signal = 0;
	}
	return (sig);
}

void	handle_signals(int sig)
{
	g_signal = sig;
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signals(t_shell *shell)
{
	struct sigaction	sa;
	sigset_t				mask;

	g_signal = 0;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	//sigaddset(&mask, SIGQUIT);
	//sigaddset(&mask, SIGTERM)
	sa.sa_mask = mask;
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_signals;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		cleanup_and_exit(shell, "failed to define critical signals\n", STDOUT_FILENO, 1);
}
