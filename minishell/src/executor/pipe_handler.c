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

static void	close_pipes(int *pipe_fds, int pipe_count)
{
    int	i;

    i = 0;
    while (i < pipe_count * 2)
    {
        close(pipe_fds[i]);
        i++;
    }
}

static void	setup_pipe_child(t_command *cmd, int *pipe_fds, int i, int pipe_count)
{
    if (i > 0) // Not first command
    {
        dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO);
    }
    if (i < pipe_count) // Not last command
    {
        dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO);
    }
    close_pipes(pipe_fds, pipe_count);
}

int	handle_pipes(t_command *cmd, t_shell *shell)
{
    int		pipe_count;
    int		*pipe_fds;
    int		i;
    pid_t	pid;
    t_command	*current;

    // Count number of pipes needed
    pipe_count = 0;
    current = cmd;
    while (current->next)
    {
        pipe_count++;
        current = current->next;
    }

    // Create pipes
    pipe_fds = malloc(sizeof(int) * (pipe_count * 2));
    if (!pipe_fds)
        return (1);
    i = 0;
    while (i < pipe_count)
    {
        if (pipe(pipe_fds + (i * 2)) < 0)
        {
            free(pipe_fds);
            return (1);
        }
        i++;
    }

    // Execute commands
    i = 0;
    current = cmd;
    while (current)
    {
        pid = fork();
        if (pid == 0)
        {
            setup_pipe_child(current, pipe_fds, i, pipe_count);
            if (handle_redirections(current) != 0)
                exit(EXIT_FAILURE);
            if (is_builtin(current->args[0]))
                exit(execute_builtin(current, shell));
            execvp(current->args[0], current->args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        current = current->next;
        i++;
    }

    // Parent process
    close_pipes(pipe_fds, pipe_count);
    free(pipe_fds);

    // Wait for all children
    while (i-- > 0)
    {
        int status;
        wait(&status);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
    }

    return (0);
}
