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

static int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

static void add_token(t_token **head, char *value, t_token_type type)
{
    t_token *new_token;
    t_token *current;

    new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return ;
    new_token->value = value;
    new_token->type = type;
    new_token->next = NULL;

    if (!*head)
        *head = new_token;
    else
    {
        current = *head;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
}

static int handle_quotes(char *input, int *i, char quote)
{
    int len;

    len = 1;  // Start after the opening quote
    while (input[*i + len] && input[*i + len] != quote)
        len++;
    
    if (!input[*i + len])  // Quote was not closed
        return (-1);
    
    return (len + 1);  // Include closing quote
}

static char *get_word(char *input, int *i)
{
    int start;
    int len;
    int quote_len;

    start = *i;
    len = 0;
    while (input[*i + len])
    {
        if (is_space(input[*i + len]) || is_operator(input[*i + len]))
            break;
        if (input[*i + len] == '\'' || input[*i + len] == '"')
        {
            quote_len = handle_quotes(input, i + len, input[*i + len]);
            if (quote_len == -1)
                return (NULL);  // Unclosed quote
            len += quote_len;
        }
        else
            len++;
    }
    *i += len;
    return (ft_substr(input, start, len));
}

static int get_operator_token(char *input, int *i, t_token **head)
{
    char *value;
    t_token_type type;

    if (input[*i] == '|')
    {
        if (input[*i + 1] == '|')
            return (0);  // Error: || not supported
        value = ft_strdup("|");
        type = TOKEN_PIPE;
        (*i)++;
    }
    else if (input[*i] == '<')
    {
        if (input[*i + 1] == '<')
        {
            if (input[*i + 2] == '<')
                return (0);  // Error: <<< invalid
            value = ft_strdup("<<");
            type = TOKEN_HEREDOC;
            (*i) += 2;
        }
        else
        {
            value = ft_strdup("<");
            type = TOKEN_REDIRECT_IN;
            (*i)++;
        }
    }
    else if (input[*i] == '>')
    {
        if (input[*i + 1] == '>')
        {
            if (input[*i + 2] == '>')
                return (0);  // Error: >>> invalid
            value = ft_strdup(">>");
            type = TOKEN_REDIRECT_APPEND;
            (*i) += 2;
        }
        else
        {
            value = ft_strdup(">");
            type = TOKEN_REDIRECT_OUT;
            (*i)++;
        }
    }
    else
        return (0);

    if (!value)
        return (0);
    add_token(head, value, type);
    return (1);
}

t_token *tokenize_input(char *input)
{
    t_token *head;
    int i;
    char *word;

    head = NULL;
    i = 0;
    while (input[i])
    {
        // Skip spaces
        while (input[i] && is_space(input[i]))
            i++;
        if (!input[i])
            break;

        // Handle operators
        if (is_operator(input[i]))
        {
            if (!get_operator_token(input, &i, &head))
            {
                free_tokens(head);
                return (NULL);
            }
            continue;
        }

        // Handle words and quotes
        word = get_word(input, &i);
        if (!word)
        {
            free_tokens(head);
            return (NULL);
        }
        add_token(&head, word, TOKEN_WORD);
    }
    return (head);
}
