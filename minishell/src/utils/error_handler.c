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

void print_error(char *msg)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putendl_fd(msg, 2);
}

void free_tokens(t_token *tokens)
{
    t_token *temp;

    while (tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}

void free_commands(t_command *commands)
{
    t_command *temp;
    int i;

    while (commands)
    {
        temp = commands;
        commands = commands->next;
        
        if (temp->args)
        {
            i = 0;
            while (temp->args[i])
                free(temp->args[i++]);
            free(temp->args);
        }
        
        if (temp->input_file)
            free(temp->input_file);
        if (temp->output_file)
            free(temp->output_file);
        
        free(temp);
    }
}

static char *get_temp_filename(void)
{
    static int heredoc_count = 0;
    char *number_str;
    char *filename;

    number_str = ft_itoa(heredoc_count++);
    if (!number_str)
        return (NULL);
    filename = ft_strjoin("/tmp/heredoc_", number_str);
    free(number_str);
    return (filename);
}

static int write_heredoc_content(int fd, char *delimiter)
{
    char *line;
    int status;

    status = EXIT_SUCCESS;
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            print_error("warning: here-document delimited by end-of-file");
            break;
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        if (write(fd, line, ft_strlen(line)) == -1 || 
            write(fd, "\n", 1) == -1)
        {
            print_error("heredoc: write error");
            status = EXIT_FAILURE;
        }
        free(line);
        if (status == EXIT_FAILURE)
            break;
    }
    return (status);
}

int handle_heredoc(t_command *cmd, char *delimiter)
{
    char *temp_file;
    int fd;
    int status;

    temp_file = get_temp_filename();
    if (!temp_file)
        return (EXIT_FAILURE);

    fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
    {
        free(temp_file);
        return (EXIT_FAILURE);
    }

    status = write_heredoc_content(fd, delimiter);
    close(fd);

    if (status == EXIT_SUCCESS)
    {
        if (cmd->input_file)
            free(cmd->input_file);
        cmd->input_file = temp_file;
    }
    else
    {
        unlink(temp_file);
        free(temp_file);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

static void cleanup_heredoc_files(void)
{
    char *temp_file;
    int i;

    i = 0;
    while (i < 1000) // reasonable limit for cleanup
    {
        temp_file = get_temp_filename();
        if (temp_file)
        {
            unlink(temp_file);
            free(temp_file);
        }
        i++;
    }
}

void free_shell(t_shell *shell)
{
    int i;

    if (shell->env)
    {
        i = 0;
        while (shell->env[i])
            free(shell->env[i++]);
        free(shell->env);
    }
    cleanup_heredoc_files();
    if (shell->commands)
        free_commands(shell->commands);
    
    if (shell->tokens)
        free_tokens(shell->tokens);
}
