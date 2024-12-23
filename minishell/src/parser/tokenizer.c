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

static char *get_content_between_quotes(char *input, int *i, char quote_type, int preserve_quotes)
{
    char *content;
    char *result;
    int start;
    int len;

    start = preserve_quotes ? *i : *i + 1;  // Start from quote or after quote
    (*i)++;  // Move past opening quote
    len = 0;

    // Count characters until closing quote
    while (input[*i + len] && input[*i + len] != quote_type)
        len++;

    if (!input[*i + len])  // No closing quote found
        return (NULL);

    if (preserve_quotes)
        len++;  // Include the closing quote if preserving

    content = ft_substr(input, start, preserve_quotes ? len + 1 : len);
    *i += len + 1;  // Move past the closing quote

    if (!content)
        return (NULL);

    if (preserve_quotes && quote_type == '\'')
    {
        // Only preserve quotes for specific cases (like echo '$USER')
        result = content;
    }
    else
    {
        result = content;
    }

    return result;
}

static char *get_unquoted_content(char *input, int *i)
{
    int start;
    int len;

    start = *i;
    len = 0;
    while (input[*i + len] && 
           !is_space(input[*i + len]) && 
           !is_operator(input[*i + len]) &&
           input[*i + len] != '\'' && 
           input[*i + len] != '"')
        len++;

    *i += len;
    return (ft_substr(input, start, len));
}

static int should_preserve_quotes(t_token *head)
{
    if (!head)
        return 1;  // Default to preserve if first token
    
    t_token *current = head;
    while (current->next)
        current = current->next;
    
    // Check if the last token is "echo"
    return (current->type == TOKEN_WORD && 
            ft_strcmp(current->value, "echo") == 0);
}

static char *get_word(char *input, int *i, t_token *head)
{
    char *result;
    char *temp;
    char *next_part;
    char quote_type;
    int preserve_quotes;

    preserve_quotes = should_preserve_quotes(head);
    result = ft_strdup("");
    if (!result)
        return (NULL);

    while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '"')
        {
            quote_type = input[*i];
            next_part = get_content_between_quotes(input, i, quote_type, preserve_quotes);
            if (!next_part)
            {
                free(result);
                return (NULL);
            }
        }
        else
        {
            next_part = get_unquoted_content(input, i);
            if (!next_part)
            {
                free(result);
                return (NULL);
            }
        }

        temp = result;
        result = ft_strjoin(result, next_part);
        free(temp);
        free(next_part);

        if (!result)
            return (NULL);
    }

    if (result[0] == '\0')
    {
        free(result);
        return (NULL);
    }
    return (result);
}

static int get_operator_token(char *input, int *i, t_token **head)
{
    char *value;
    t_token_type type;

    if (input[*i] == '|')
    {
        value = ft_strdup("|");
        type = TOKEN_PIPE;
        (*i)++;
    }
    else if (input[*i] == '<')
    {
        if (input[*i + 1] == '<')
        {
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
        while (input[i] && is_space(input[i]))
            i++;
        if (!input[i])
            break;

        if (is_operator(input[i]))
        {
            if (!get_operator_token(input, &i, &head))
            {
                free_tokens(head);
                return (NULL);
            }
            continue;
        }

        word = get_word(input, &i, head);
        if (!word)
        {
            if (input[i] == '\'' || input[i] == '"')  // Unclosed quote
            {
                free_tokens(head);
                return (NULL);
            }
            continue;  // Skip empty words
        }
        add_token(&head, word, TOKEN_WORD);
    }
    return (head);
}
