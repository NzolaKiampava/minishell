/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:39:08 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 10:39:12 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int handle_redirections(t_command *cmd)
{
    int saved_stdout;
    int saved_stdin;

    saved_stdout = dup(STDOUT_FILENO);
    saved_stdin = dup(STDIN_FILENO);
    
    if (saved_stdout == -1 || saved_stdin == -1)
    {
        if (saved_stdout != -1)
            close(saved_stdout);
        if (saved_stdin != -1)
            close(saved_stdin);
        return (0);
    }

    // Handle input redirection
    if (cmd->input_file)
    {
        if (cmd->input_fd == -2)  // heredoc
        {
            cmd->input_fd = handle_heredoc(cmd);
            if (cmd->input_fd == -1)
            {
                close(saved_stdout);
                close(saved_stdin);
                return (0);
            }
        }
        else
        {
            cmd->input_fd = open(cmd->input_file, O_RDONLY);
            if (cmd->input_fd == -1)
            {
                print_error("No such file or directory");
                close(saved_stdout);
                close(saved_stdin);
                return (0);
            }
        }
        if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
        {
            close(cmd->input_fd);
            close(saved_stdout);
            close(saved_stdin);
            return (0);
        }
        close(cmd->input_fd);
    }

    // Handle output redirection
    if (cmd->output_file)
    {
        int flags = O_WRONLY | O_CREAT;
        flags |= (cmd->append_mode ? O_APPEND : O_TRUNC);
        
        cmd->output_fd = open(cmd->output_file, flags, 0644);
        if (cmd->output_fd == -1)
        {
            print_error("Cannot open output file");
            if (cmd->input_file)
                dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdout);
            close(saved_stdin);
            return (0);
        }

        if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
        {
            print_error("Failed to redirect output");
            close(cmd->output_fd);
            if (cmd->input_file)
                dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdout);
            close(saved_stdin);
            return (0);
        }

        close(cmd->output_fd);
    }

    close(saved_stdout);
    close(saved_stdin);
    return (1);
}
