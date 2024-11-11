/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:09:39 by nkiampav          #+#    #+#             */
/*   Updated: 2024/11/11 14:16:54 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	init_shell(void)
{
	setup_signals();
	// initialize readline
	usign_history();
}

int	main(void)
{
	char	*input;
	t_command	*cmd;

	init_shell();

	while (1)
	{
		input = read_input();
		if (!input)  // handle ctrl+D
			break;
		add_to_history(input);

		if (parse_input(input, &cmd) == 0)
		{
			execute_command(cmd);
			free_command(cmd);
		}
		free(input);
	}
	printf("exit\n");
	rl_clear_history();
	return (0);
}
