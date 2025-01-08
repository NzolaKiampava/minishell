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

void handle_signal(int signo)
{
    if (signo == SIGINT)
    {
        g_signal_received = 1;
        write(STDOUT_FILENO, "\n", 1);
    }
}

void handle_parent_signal(int signo)
{
    if (signo == SIGINT)
    {
        g_signal_received = 1;
        write(STDOUT_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

void setup_signals(void)
{
    struct sigaction sa;
    
    sa.sa_handler = handle_parent_signal;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
