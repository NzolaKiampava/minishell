/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-05 21:26:56 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-05 21:26:56 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_tokens(t_token *tokens) {
    t_token *current;
    
    while (tokens) {
        current = tokens;
        tokens = tokens->next;
        
        // Free the value of each token
        if (current->value) {
            free(current->value);
        }
        
        // Free the token itself
        free(current);
    }
}

void free_commands(t_command *commands) {
    t_command *current;
    int i;
    
    while (commands) {
        current = commands;
        commands = commands->next;
        
        // Free arguments
        if (current->args) {
            for (i = 0; current->args[i]; i++) {
                free(current->args[i]);
            }
            free(current->args);
        }
        
        // Free input and output file names
        if (current->input_file) {
            free(current->input_file);
        }
        if (current->output_file) {
            free(current->output_file);
        }
        
        // Free the command structure
        free(current);
    }
}

void free_shell(t_shell *shell) {
    // Free environment variables
    if (shell->env) {
        for (int i = 0; shell->env[i]; i++) {
            free(shell->env[i]);
        }
        free(shell->env);
    }
    
    // Free tokens
    if (shell->tokens) {
        free_tokens(shell->tokens);
    }
    
    // Free commands
    if (shell->commands) {
        free_commands(shell->commands);
    }
    
    // Reset shell structure
    shell->env = NULL;
    shell->tokens = NULL;
    shell->commands = NULL;
    shell->exit_status = 0;
    shell->running = 0;
}

// Print error message to stderr
void print_error(char *msg) {
    if (msg) {
        // Prefix the error message with the program name
        fprintf(stderr, "minishell: %s\n", msg);
    }
}