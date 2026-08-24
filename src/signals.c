/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 14:36:06 by danimend          #+#    #+#             */
/*   Updated: 2026/08/24 14:36:18 by danimend         ###   ########.fr       */
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

static int	install_interactive_handlers(void)
{
	struct sigaction	sa;
	sigset_t			mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sa.sa_mask = mask;
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_signals;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (0);
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		return (0);
	return (1);
}

void	setup_signals(t_shell *shell)
{
	g_signal = 0;
	if (!install_interactive_handlers())
		cleanup_and_exit(shell, "failed to define critical signals\n",
			STDERR_FILENO, 1);
}

void	restore_signals(void)
{
	install_interactive_handlers();
}
