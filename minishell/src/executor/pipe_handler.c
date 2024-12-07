/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:28:04 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 10:28:06 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void close_pipes(int pipes[][2], int pipe_count)
{
    int i;

    i = 0;
    while (i < pipe_count)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

static int setup_pipes(int pipes[][2], int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        if (pipe(pipes[i]) < 0)
        {
            while (i > 0)
            {
                i--;
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
            return (0);
        }
        i++;
    }
    return (1);
}

static void setup_child_pipes(int pipes[][2], int i, int pipe_count)
{
    if (i > 0) // Not first command
    {
        dup2(pipes[i - 1][0], STDIN_FILENO);
    }
    
    if (i < pipe_count) // Not last command
    {
        dup2(pipes[i][1], STDOUT_FILENO);
    }

    close_pipes(pipes, pipe_count);
}

int handle_pipes(t_command *cmd, t_shell *shell)
{
    int pipe_count;
    int pipes[1024][2];
    t_command *current;
    pid_t *pids;
    int i, status, last_status;

    // Count commands and allocate pid array
    pipe_count = 0;
    current = cmd;
    while (current->next && pipe_count < 1024)
    {
        pipe_count++;
        current = current->next;
    }

    pids = (pid_t *)malloc(sizeof(pid_t) * (pipe_count + 1));
    if (!pids || !setup_pipes(pipes, pipe_count))
        return (1);

    // Execute commands
    i = 0;
    current = cmd;
    while (current)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            setup_child_pipes(pipes, i, pipe_count);
            
            if (handle_redirections(current) != 0)
                exit(1);

            if (is_builtin(current->args[0]))
                exit(execute_builtin(current, shell));
            
            if (!current->args[0])
                exit(0);

            char *cmd_path = find_command_path(current->args[0], shell->env);
            if (!cmd_path)
                exit(127);

            execve(cmd_path, current->args, shell->env);
            free(cmd_path);
            exit(1);
        }
        current = current->next;
        i++;
    }

    close_pipes(pipes, pipe_count);

    // Wait for all children and get last status
    last_status = 0;
    for (i = 0; i <= pipe_count; i++)
    {
        waitpid(pids[i], &status, 0);
        if (i == pipe_count)
            last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }

    free(pids);
    return (last_status);
}
