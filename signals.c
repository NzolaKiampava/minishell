/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:18:07 by nkiampav          #+#    #+#             */
/*   Updated: 2024/11/11 15:25:14 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals(void)
{
	struct sigaction	sa;

	// Initialize sigaction struct
	sigemptyset(&sa.a_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_signal;

	// Set up signal handlers
	sigaction(SIGINT, &sa, NULL);  // ctrl+c
	sigaction(SIGQUIT, &sa, NULL); // ctrl+\
}

void	handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = SIGINT;
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
		g_signal = SIGQUIT;
}
