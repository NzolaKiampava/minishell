/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:17:49 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 10:17:51 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

static int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static t_token *create_token(char *value, t_token_type type)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = ft_strdup(value);
    if (!token->value)
    {
        free(token);
        return (NULL);
    }
    token->type = type;
    token->next = NULL;
    return (token);
}

static void add_token(t_token **head, t_token *new_token)
{
    t_token *current;

    if (!*head)
    {
        *head = new_token;
        return;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

static char *handle_quotes(char *input, int *i, char quote_type)
{
    int start;
    int len;
    char *value;

    start = *i + 1;
    *i = start;
    while (input[*i] && input[*i] != quote_type)
        (*i)++;
    
    if (!input[*i])  // Unclosed quote
        return (NULL);
    
    len = *i - start;
    value = (char *)malloc(sizeof(char) * (len + 1));
    if (!value)
        return (NULL);
    ft_strlcpy(value, input + start, len + 1);
    return (value);
}

static t_token_type get_operator_type(char *input, int *i)
{
    if (input[*i] == '|')
        return (TOKEN_PIPE);
    else if (input[*i] == '<')
    {
        if (input[*i + 1] == '<')
        {
            (*i)++;
            return (TOKEN_HEREDOC);
        }
        return (TOKEN_REDIRECT_IN);
    }
    else if (input[*i] == '>')
    {
        if (input[*i + 1] == '>')
        {
            (*i)++;
            return (TOKEN_REDIRECT_APPEND);
        }
        return (TOKEN_REDIRECT_OUT);
    }
    return (TOKEN_WORD);  // Should never happen
}

t_token *tokenize_input(char *input)
{
    t_token *tokens = NULL;
    int i = 0;
    int start;
    char *word;
    t_token_type type;

    while (input[i])
    {
        // Skip whitespace
        while (input[i] && is_whitespace(input[i]))
            i++;
        
        if (!input[i])
            break;

        // Handle operators
        if (is_operator(input[i]))
        {
            type = get_operator_type(input, &i);
            word = (char *)malloc(sizeof(char) * 3);
            if (!word)
                return (NULL);
            if (type == TOKEN_HEREDOC || type == TOKEN_REDIRECT_APPEND)
            {
                word[0] = input[i - 1];
                word[1] = input[i];
                word[2] = '\0';
            }
            else
            {
                word[0] = input[i];
                word[1] = '\0';
            }
            add_token(&tokens, create_token(word, type));
            free(word);
            i++;
            continue;
        }

        // Handle quotes
        if (input[i] == '\'' || input[i] == '"')
        {
            word = handle_quotes(input, &i, input[i]);
            if (!word)
            {
                free_tokens(tokens);
                return (NULL);
            }
            add_token(&tokens, create_token(word, TOKEN_WORD));
            free(word);
            i++;
            continue;
        }

        // Handle regular words
        start = i;
        while (input[i] && !is_whitespace(input[i]) && !is_operator(input[i]) 
               && input[i] != '\'' && input[i] != '"')
            i++;
        
        word = (char *)malloc(sizeof(char) * (i - start + 1));
        if (!word)
            return (NULL);
        ft_strlcpy(word, input + start, i - start + 1);
        add_token(&tokens, create_token(word, TOKEN_WORD));
        free(word);
    }

    return (tokens);
}
