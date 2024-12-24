/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-05 21:50:09 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-05 21:50:09 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int signo)
{
	if (signo == SIGINT) 
	{
		// Handle CTRL+C
		g_signal_received = 1;
		printf("\n");
		rl_replace_line("", 0);  // Clear the current input line
		rl_on_new_line();        // Move to a new line
		rl_redisplay();          // Redisplay the prompt
	}
}

void	setup_signals(void)
{
	// Handle SIGINT (CTRL+C)
	signal(SIGINT, handle_signal);

	// Ignore SIGQUIT (CTRL+\)
	signal(SIGQUIT, SIG_IGN);
}
