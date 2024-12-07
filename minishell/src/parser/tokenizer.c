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

static int get_operator_type(char *input, int *i)
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
    return (TOKEN_WORD);
}

static t_token *create_token(char *value, t_token_type type)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = value;
    token->type = type;
    token->next = NULL;
    return (token);
}

static char *get_word(char *input, int *i)
{
    int start;
    int len;
    char quote;

    start = *i;
    len = 0;
    quote = 0;

    while (input[*i + len])
    {
        if (!quote && (is_space(input[*i + len]) || is_operator(input[*i + len])))
            break;
        if (!quote && (input[*i + len] == '\'' || input[*i + len] == '"'))
            quote = input[*i + len];
        else if (quote && input[*i + len] == quote)
            quote = 0;
        len++;
    }

    *i += len;
    return (ft_substr(input, start, len));
}

t_token *tokenize_input(char *input)
{
    t_token *head;
    t_token *current;
    int i;
    char *word;
    t_token_type type;

    head = NULL;
    current = NULL;
    i = 0;

    while (input[i])
    {
        while (input[i] && is_space(input[i]))
            i++;
        if (!input[i])
            break;

        if (is_operator(input[i]))
        {
            type = get_operator_type(input, &i);
            word = ft_substr(input, i - (type == TOKEN_HEREDOC || 
                type == TOKEN_REDIRECT_APPEND ? 2 : 1), 
                type == TOKEN_HEREDOC || type == TOKEN_REDIRECT_APPEND ? 2 : 1);
        }
        else
        {
            type = TOKEN_WORD;
            word = get_word(input, &i);
        }

        if (!word)
        {
            free_tokens(head);
            return (NULL);
        }

        if (!head)
            head = create_token(word, type);
        else
        {
            current = head;
            while (current->next)
                current = current->next;
            current->next = create_token(word, type);
        }
    }
    return (head);
}
