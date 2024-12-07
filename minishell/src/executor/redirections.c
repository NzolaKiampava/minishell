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

static int open_input_file(t_command *cmd)
{
    if (cmd->input_file)
    {
        cmd->input_fd = open(cmd->input_file, O_RDONLY);
        if (cmd->input_fd < 0)
        {
            print_error(strerror(errno));
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

static int open_output_file(t_command *cmd)
{
    int flags;

    if (cmd->output_file)
    {
        flags = O_WRONLY | O_CREAT;
        if (cmd->append_mode)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
        cmd->output_fd = open(cmd->output_file, flags, 0644);
        if (cmd->output_fd < 0)
        {
            print_error(strerror(errno));
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

static int setup_redirections(t_command *cmd)
{
    if (cmd->input_fd != STDIN_FILENO)
    {
        if (dup2(cmd->input_fd, STDIN_FILENO) < 0)
        {
            print_error(strerror(errno));
            return (EXIT_FAILURE);
        }
        close(cmd->input_fd);
    }
    if (cmd->output_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->output_fd, STDOUT_FILENO) < 0)
        {
            print_error(strerror(errno));
            return (EXIT_FAILURE);
        }
        close(cmd->output_fd);
    }
    return (EXIT_SUCCESS);
}

static void cleanup_redirections(t_command *cmd)
{
    if (cmd->input_fd != STDIN_FILENO)
        close(cmd->input_fd);
    if (cmd->output_fd != STDOUT_FILENO)
        close(cmd->output_fd);
}

int handle_redirections(t_command *cmd)
{
    int status;

    // Initialize file descriptors to standard input/output
    cmd->input_fd = STDIN_FILENO;
    cmd->output_fd = STDOUT_FILENO;

    // Open input file if specified
    if ((status = open_input_file(cmd)) != EXIT_SUCCESS)
    {
        cleanup_redirections(cmd);
        return (status);
    }

    // Open output file if specified
    if ((status = open_output_file(cmd)) != EXIT_SUCCESS)
    {
        cleanup_redirections(cmd);
        return (status);
    }

    // Setup redirections (dup2)
    if ((status = setup_redirections(cmd)) != EXIT_SUCCESS)
    {
        cleanup_redirections(cmd);
        return (status);
    }

    return (EXIT_SUCCESS);
}
