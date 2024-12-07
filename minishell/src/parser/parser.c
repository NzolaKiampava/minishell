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
    new_args[i + 1] = NULL;

    if (cmd->args)
        free(cmd->args);
    cmd->args = new_args;
    return (1);
}

static int handle_redirect(t_command *cmd, t_token **token)
{
    t_token_type type;
    
    type = (*token)->type;
    *token = (*token)->next;
    
    if (!*token || (*token)->type != TOKEN_WORD)
    {
        print_error("syntax error near unexpected token");
        return (0);
    }

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
        if (handle_heredoc(cmd, (*token)->value) != EXIT_SUCCESS)
            return (0);
    }

    *token = (*token)->next;
    return (1);
}

t_command *parse_tokens(t_token *tokens)
{
    t_command *head;
    t_command *current;
    t_token *token;

    head = NULL;
    current = NULL;
    token = tokens;

    while (token)
    {
        if (!head)
        {
            head = create_command();
            current = head;
        }
        else if (token->type == TOKEN_PIPE)
        {
            current->next = create_command();
            current = current->next;
            token = token->next;
            continue;
        }

        if (!current)
            return (NULL);

        if (token->type == TOKEN_WORD)
        {
            if (!add_arg(current, token->value))
                return (NULL);
            token = token->next;
        }
        else if (token->type >= TOKEN_REDIRECT_IN && token->type <= TOKEN_HEREDOC)
        {
            if (!handle_redirect(current, &token))
                return (NULL);
        }
        else
            token = token->next;
    }

    return (head);
}
