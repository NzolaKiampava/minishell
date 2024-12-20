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
    cmd->input_fd = -1;  // Changed from STDIN_FILENO
    cmd->output_fd = -1; // Changed from STDOUT_FILENO
    cmd->append_mode = 0;
    cmd->next = NULL;
    return (cmd);
}

static void free_command(t_command *cmd)
{
    int i;

    if (!cmd)
        return;
    if (cmd->args)
    {
        i = 0;
        while (cmd->args[i])
            free(cmd->args[i++]);
        free(cmd->args);
    }
    if (cmd->input_file)
        free(cmd->input_file);
    if (cmd->output_file)
        free(cmd->output_file);
    free(cmd);
}

static int add_arg(t_command *cmd, char *arg)
{
    char **new_args;
    int i;
    int size;

    size = 0;
    if (cmd->args)
        while (cmd->args[size])
            size++;

    new_args = (char **)malloc(sizeof(char *) * (size + 2));
    if (!new_args)
        return (0);

    i = 0;
    while (i < size)
    {
        new_args[i] = cmd->args[i];
        i++;
    }
    new_args[i] = ft_strdup(arg);
    if (!new_args[i])
    {
        free(new_args);
        return (0);
    }
    new_args[i + 1] = NULL;

    if (cmd->args)
        free(cmd->args);
    cmd->args = new_args;
    return (1);
}

static int validate_redirect_syntax(t_token *token)
{
    // Check if there's a next token
    if (!token->next)
        return (0);
    
    // Check if the next token is a word
    if (token->next->type != TOKEN_WORD)
        return (0);
    
    // Check if we have multiple redirects of the same type consecutively
    if (token->next->next && 
        (token->next->next->type == TOKEN_REDIRECT_IN ||
         token->next->next->type == TOKEN_REDIRECT_OUT ||
         token->next->next->type == TOKEN_REDIRECT_APPEND ||
         token->next->next->type == TOKEN_HEREDOC))
        return (0);
    
    return (1);
}

static int handle_redirect(t_command *cmd, t_token **token)
{
    t_token_type type;
    
    if (!validate_redirect_syntax(*token))
    {
        print_error("syntax error near unexpected token");
        return (0);
    }

    type = (*token)->type;
    *token = (*token)->next; // Move to the filename/delimiter token

    if (type == TOKEN_REDIRECT_IN)
    {
        if (cmd->input_file)
            free(cmd->input_file);
        cmd->input_file = ft_strdup((*token)->value);
        cmd->input_fd = -1;  // Will be opened later
    }
    else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND)
    {
        if (cmd->output_file)
            free(cmd->output_file);
        cmd->output_file = ft_strdup((*token)->value);
        cmd->output_fd = -1;  // Will be opened later
        cmd->append_mode = (type == TOKEN_REDIRECT_APPEND);
    }
    else if (type == TOKEN_HEREDOC)
    {
        if (cmd->input_file)
            free(cmd->input_file);
        cmd->input_file = ft_strdup((*token)->value);
        cmd->input_fd = -2;  // Special value for heredoc
    }

    if (!cmd->input_file && !cmd->output_file)
        return (0);

    *token = (*token)->next;
    return (1);
}

static int validate_pipe_syntax(t_token *tokens)
{
    // Check if pipe is first token
    if (tokens->type == TOKEN_PIPE)
        return (0);

    while (tokens)
    {
        // Check for consecutive pipes
        if (tokens->type == TOKEN_PIPE && 
            (!tokens->next || tokens->next->type == TOKEN_PIPE))
            return (0);
        tokens = tokens->next;
    }
    return (1);
}

t_command *parse_tokens(t_token *tokens)
{
    t_command *head;
    t_command *current;
    t_token *token;

    if (!validate_pipe_syntax(tokens))
    {
        print_error("syntax error near unexpected token '|'");
        return (NULL);
    }

    head = NULL;
    current = NULL;
    token = tokens;

    while (token)
    {
        if (!head)
        {
            head = create_command();
            current = head;
            if (!current)
                return (NULL);
        }
        else if (token->type == TOKEN_PIPE)
        {
            if (!current->args)
            {
                print_error("syntax error near unexpected token '|'");
                return (NULL);
            }
            current->next = create_command();
            if (!current->next)
                return (NULL);
            current = current->next;
            token = token->next;
            continue;
        }

        if (token->type == TOKEN_WORD)
        {
            if (!add_arg(current, token->value))
            {
                // Cleanup and return
                while (head)
                {
                    current = head->next;
                    free_command(head);
                    head = current;
                }
                return (NULL);
            }
            token = token->next;
        }
        else if (token->type >= TOKEN_REDIRECT_IN && token->type <= TOKEN_HEREDOC)
        {
            if (!handle_redirect(current, &token))
            {
                // Cleanup and return
                while (head)
                {
                    current = head->next;
                    free_command(head);
                    head = current;
                }
                return (NULL);
            }
        }
        else
            token = token->next;
    }

    return (head);
}
