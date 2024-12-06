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

// Global variable for signal handling
int g_signal_received = 0;

void init_shell(t_shell *shell, char **env) {
    shell->env = init_env(env);
    shell->commands = NULL;
    shell->tokens = NULL;
    shell->exit_status = 0;
    shell->running = 1;
}

void handle_input(t_shell *shell, char *input) {
    if (input == NULL) {
        // Handle CTRL+D (EOF)
        shell->running = 0;
        printf("\n");
        return;
    }

    // Only add non-empty inputs to history
    if (input && *input) {
        add_history(input);
    }

    // TODO: Implement tokenization, parsing, and execution
    // For now, just print the input
    printf("Received: %s\n", input);

    // Free the input
    free(input);
}

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)), char **env) {
    t_shell shell;

    // Initialize shell
    init_shell(&shell, env);

    // Setup signal handling
    setup_signals();

    // Main shell loop
    while (shell.running) {
        // Reset global signal flag
        g_signal_received = 0;

        // Display prompt
        char *input = readline("minishell$ ");

        // Handle input
        handle_input(&shell, input);
    }

    // Cleanup
    free_shell(&shell);

    return shell.exit_status;
}
