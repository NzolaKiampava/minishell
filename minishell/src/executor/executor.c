/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:41:31 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 12:41:33 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int count_commands(t_command *cmd)
{
    int count;
    t_command *current;

    count = 0;
    current = cmd;
    while (current)
    {
        count++;
        current = current->next;
    }
    return (count);
}

static void cleanup_command(t_command *cmd)
{
    if (cmd->input_fd != STDIN_FILENO)
        close(cmd->input_fd);
    if (cmd->output_fd != STDOUT_FILENO)
        close(cmd->output_fd);
}

static int execute_single_command(t_command *cmd, t_shell *shell)
{
    pid_t   pid;
    int     status;
    char    *path;

    // Handle builtin commands
    if (cmd->args && is_builtin(cmd->args[0]))
        return (execute_builtin(cmd, shell));

    pid = fork();
    if (pid < 0)
        return (EXIT_FAILURE);

    if (pid == 0)
    {
        // Child process
        if (handle_redirections(cmd) != 0)
            exit(EXIT_FAILURE);

        if (!cmd->args || !cmd->args[0])
            exit(EXIT_SUCCESS);

        path = ft_strjoin("/bin/", cmd->args[0]);
        if (execve(path, cmd->args, shell->env) < 0)
        {
            free(path);
            path = ft_strjoin("/usr/bin/", cmd->args[0]);
            if (execve(path, cmd->args, shell->env) < 0)
            {
                free(path);
                print_error("command not found");
                exit(127);
            }
        }
        free(path);
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (EXIT_FAILURE);
    }
}

int execute_commands(t_shell *shell)
{
    int cmd_count;
    t_command *current;

    if (!shell || !shell->commands)
        return (EXIT_FAILURE);

    // Expand variables in all commands
    current = shell->commands;
    while (current)
    {
        expand_variables(current, shell);
        current = current->next;
    }

    cmd_count = count_commands(shell->commands);

    // If we have multiple commands, use pipe handler
    if (cmd_count > 1)
        return (handle_pipes(shell->commands, shell));

    // For single command, execute directly
    shell->exit_status = execute_single_command(shell->commands, shell);
    cleanup_command(shell->commands);
    return (shell->exit_status);
}

static char *get_command_path(char *cmd)
{
    char *path;
    char *full_path;
    struct stat s;

    // First try direct path
    if (stat(cmd, &s) == 0)
        return (ft_strdup(cmd));

    // Try /bin/
    path = ft_strjoin("/bin/", cmd);
    if (stat(path, &s) == 0)
        return (path);
    free(path);

    // Try /usr/bin/
    path = ft_strjoin("/usr/bin/", cmd);
    if (stat(path, &s) == 0)
        return (path);
    free(path);

    return (NULL);
}

int execute_external_command(char **args, char **envp)
{
    char *cmd_path;

    if (!args || !args[0])
        return (EXIT_FAILURE);

    cmd_path = get_command_path(args[0]);
    if (!cmd_path)
    {
        print_error("command not found");
        return (127);
    }

    if (execve(cmd_path, args, envp) < 0)
    {
        free(cmd_path);
        print_error("execution failed");
        return (EXIT_FAILURE);
    }

    // Should never reach here
    free(cmd_path);
    return (EXIT_FAILURE);
}
