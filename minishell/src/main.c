/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:09:39 by nkiampav          #+#    #+#             */
/*   Updated: 2024/11/11 14:16:54 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_received = 0;

static void	initialize_shell(t_shell *shell, char **env)
{
	shell->env = init_env(env);
	shell->commands = NULL;
	shell->tokens = NULL;
	shell->exit_status = 0;
	shell->running = 1;
}

static void	process_input(t_shell *shell, char *input)
{
	if (!input)
	{
		printf("exit\n");
		shell->running = 0;
		return;
	}

	if (ft_strlen(input) > 0)
	{
		add_history(input);
		shell->tokens = tokenize_input(input);

		if (shell->tokens)
		{
			shell->commands = parse_tokens(shell->tokens);
			if (shell->commands)
			{
				expand_variables(shell->commands, shell);
				shell->exit_status = execute_commands(shell);
			}
			free_commands(shell->commands);
			shell->commands = NULL;
		}
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	free(input);
}

int	main(int argc, char **argv, char **env)
{
	t_shell		shell;
	char		*input;

	(void)argc;
	(void)argv;

	// Initialize shell
	initialize_shell(&shell, env);

	// Setup signal handlers
	setup_signals();

	// Main shell loop
	while (shell.running)
	{
		input = readline("minishell$ ");
		process_input(&shell, input);
	}

	// Cleanup
	free_shell(&shell);
	return (shell.exit_status);
}
