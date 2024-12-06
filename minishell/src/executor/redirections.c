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

static int	handle_input_redirect(t_command *cmd)
{
    int	fd;

    if (cmd->input_file)
    {
        fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            return (1);
        }
        if (dup2(fd, STDIN_FILENO) < 0)
        {
            perror("dup2");
            close(fd);
            return (1);
        }
        close(fd);
    }
    return (0);
}

static int	handle_output_redirect(t_command *cmd)
{
    int	fd;
    int	flags;

    if (cmd->output_file)
    {
        flags = O_WRONLY | O_CREAT;
        if (cmd->append_mode)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
        
        fd = open(cmd->output_file, flags, 0644);
        if (fd < 0)
        {
            perror("open");
            return (1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("dup2");
            close(fd);
            return (1);
        }
        close(fd);
    }
    return (0);
}

int	handle_redirections(t_command *cmd)
{
    if (handle_input_redirect(cmd) != 0)
        return (1);
    if (handle_output_redirect(cmd) != 0)
        return (1);
    return (0);
}
