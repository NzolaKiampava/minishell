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

// Função para encontrar o caminho completo do executável
char *get_command_path(char *cmd, char **env)
{
    char *path;
    char **paths;
    char *full_path;
    int i;

    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd));
    path = get_env_value(env, "PATH");
    if (!path)
        return (NULL);
    paths = ft_split(path, ':');
    if (!paths)
        return (NULL);
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin_three(paths[i], "/", cmd);
        if (!access(full_path, X_OK))
        {
            ft_free_array(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    ft_free_array(paths);
    return (NULL);
}

/*
static void	close_pipe_fds(int *pipe_fds)
{
    if (pipe_fds[0] != -1)
        close(pipe_fds[0]);
    if (pipe_fds[1] != -1)
        close(pipe_fds[1]);
}
*/

static int	setup_pipe(t_command *cmd, int *prev_pipe, int *curr_pipe)
{
    if (prev_pipe[0] != -1)
    {
        if (dup2(prev_pipe[0], STDIN_FILENO) == -1)
            return (0);
        close(prev_pipe[0]);
        prev_pipe[0] = -1;
    }

    if (cmd->next)
    {
        if (pipe(curr_pipe) == -1)
            return (0);
        if (dup2(curr_pipe[1], STDOUT_FILENO) == -1)
            return (0);
        close(curr_pipe[1]);
        curr_pipe[1] = -1;
    }
    return (1);
}

static int	handle_heredoc(t_command *cmd)
{
    int		pipe_fd[2];
    char	*line;
    char	*delimiter;

    if (pipe(pipe_fd) == -1)
        return (-1);
    delimiter = cmd->input_file;
    while (1)
    {
        line = readline("> ");
        if (!line || !ft_strcmp(line, delimiter))
        {
            free(line);
            break;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    return (pipe_fd[0]);
}

int	handle_redirections(t_command *cmd)
{
    if (cmd->input_file)
    {
        if (cmd->input_fd == -2)  // heredoc
        {
            cmd->input_fd = handle_heredoc(cmd);
            if (cmd->input_fd == -1)
                return (0);
        }
        else
        {
            cmd->input_fd = open(cmd->input_file, O_RDONLY);
            if (cmd->input_fd == -1)
            {
                print_error("No such file or directory");
                return (0);
            }
        }
        if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
            return (0);
        close(cmd->input_fd);
    }

    if (cmd->output_file)
    {
        cmd->output_fd = open(cmd->output_file, 
            O_WRONLY | O_CREAT | (cmd->append_mode ? O_APPEND : O_TRUNC), 
            0644);
        if (cmd->output_fd == -1)
        {
            print_error("Cannot open output file");
            return (0);
        }
        if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
            return (0);
        close(cmd->output_fd);
    }
    return (1);
}

static int	execute_single_command(t_command *cmd, t_shell *shell)
{
    pid_t	pid;
    int		status;
    char	*path;

    if (is_builtin(cmd->args[0]))
        return (execute_builtin(cmd, shell));

    pid = fork();
    if (pid == -1)
        return (1);
    if (pid == 0)
    {
        if (!handle_redirections(cmd))
            exit(1);
        path = get_command_path(cmd->args[0], shell->env);
        if (!path)
        {
            print_error("Command not found");
            exit(127);
        }
        execve(path, cmd->args, shell->env);
        free(path);
        exit(1);
    }
    waitpid(pid, &status, 0);
    return (WEXITSTATUS(status));
}

static int	execute_piped_commands(t_command *cmd, t_shell *shell)
{
    pid_t	pid;
    int		prev_pipe[2] = {-1, -1};
    int		curr_pipe[2] = {-1, -1};
    int		status;
    int		last_status;

    while (cmd)
    {
        pid = fork();
        if (pid == -1)
            return (1);
        if (pid == 0)
        {
            if (!setup_pipe(cmd, prev_pipe, curr_pipe))
                exit(1);
            if (!handle_redirections(cmd))
                exit(1);
            if (is_builtin(cmd->args[0]))
                exit(execute_builtin(cmd, shell));
            execve(get_command_path(cmd->args[0], shell->env), 
                cmd->args, shell->env);
            exit(1);
        }
        if (prev_pipe[0] != -1)
            close(prev_pipe[0]);
        if (curr_pipe[1] != -1)
            close(curr_pipe[1]);
        prev_pipe[0] = curr_pipe[0];
        prev_pipe[1] = curr_pipe[1];
        cmd = cmd->next;
    }
    
    while (wait(&status) > 0)
        last_status = WEXITSTATUS(status);
    return (last_status);
}

int	execute_commands(t_shell *shell)
{
    t_command	*cmd;
    int			has_pipe;

    cmd = shell->commands;
    if (!cmd)
        return (0);

    // Check if we have any pipes
    has_pipe = 0;
    for (t_command *tmp = cmd; tmp->next; tmp = tmp->next)
        has_pipe = 1;

    if (has_pipe)
        return (execute_piped_commands(cmd, shell));
    else
        return (execute_single_command(cmd, shell));
}
