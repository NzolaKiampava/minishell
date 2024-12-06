/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 09:43:37 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 09:43:41 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command *create_command(void)
{
    t_command *cmd;

    cmd = (t_command *)malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->input_fd = STDIN_FILENO;
    cmd->output_fd = STDOUT_FILENO;
    cmd->append_mode = 0;
    cmd->next = NULL;
    return (cmd);
}

static void add_command(t_command **head, t_command *new_cmd)
{
    t_command *current;

    if (!*head)
    {
        *head = new_cmd;
        return;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_cmd;
}

static int add_argument(t_command *cmd, char *arg)
{
    int i;
    char **new_args;

    i = 0;
    if (cmd->args)
    {
        while (cmd->args[i])
            i++;
    }
    new_args = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return (0);
    
    i = 0;
    if (cmd->args)
    {
        while (cmd->args[i])
        {
            new_args[i] = cmd->args[i];
            i++;
        }
        free(cmd->args);
    }
    new_args[i] = ft_strdup(arg);
    new_args[i + 1] = NULL;
    cmd->args = new_args;
    return (1);
}

static int handle_redirect(t_command *cmd, t_token **token)
{
    t_token_type type;

    type = (*token)->type;
    *token = (*token)->next;
    if (!*token || (*token)->type != TOKEN_WORD)
        return (0);

    if (type == TOKEN_REDIRECT_IN)
    {
        if (cmd->input_file)
            free(cmd->input_file);
        cmd->input_file = ft_strdup((*token)->value);
    }
    else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND)
    {
        if (cmd->output_file)
            free(cmd->output_file);
        cmd->output_file = ft_strdup((*token)->value);
        cmd->append_mode = (type == TOKEN_REDIRECT_APPEND);
    }
    else if (type == TOKEN_HEREDOC)
    {
        // TODO: Implement heredoc handling
        if (handle_heredoc(cmd, (*token)->value) < 0)
        	return (0);
    }
    *token = (*token)->next;
    return (1);
}

t_command *parse_tokens(t_token *tokens)
{
    t_command *commands = NULL;
    t_command *current_cmd = NULL;
    t_token *current = tokens;

    while (current)
    {
        if (!current_cmd)
        {
            current_cmd = create_command();
            if (!current_cmd)
                return (NULL);
            add_command(&commands, current_cmd);
        }

        if (current->type == TOKEN_WORD)
        {
            if (!add_argument(current_cmd, current->value))
            {
                free_commands(commands);
                return (NULL);
            }
            current = current->next;
        }
        else if (current->type == TOKEN_PIPE)
        {
            current_cmd = NULL;
            current = current->next;
        }
        else if (current->type == TOKEN_REDIRECT_IN || 
                 current->type == TOKEN_REDIRECT_OUT ||
                 current->type == TOKEN_REDIRECT_APPEND ||
                 current->type == TOKEN_HEREDOC)
        {
            if (!handle_redirect(current_cmd, &current))
            {
                free_commands(commands);
                return (NULL);
            }
        }
        else
        {
            free_commands(commands);
            return (NULL);
        }
    }

    return (commands);
}
