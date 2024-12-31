static void handle_input_processing(t_shell *shell, char *input)
{
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
                free_commands(shell->commands);
                shell->commands = NULL;
            }
            free_tokens(shell->tokens);
            shell->tokens = NULL;
        }
    }
}

static void process_input(t_shell *shell, char *input)
{
    if (!input)
    {
        printf("exit\n");
        shell->running = 0;
        return;
    }
    handle_input_processing(shell, input);
    free(input);
}
