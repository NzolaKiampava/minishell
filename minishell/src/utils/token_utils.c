/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-30 14:31:27 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-30 14:31:27 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_token **head, char *value, t_token_type type)
{
	t_token	*new_token;
	t_token	*current;

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

char	*prepare_quote_extraction(char *input, int *i, int preserve_quotes,
	int *start)
{
	if (preserve_quotes)
		*start = *i;
	else
		*start = *i + 1;
	(*i)++;
	return (input);
}

char	*get_content_between_quotes(char *input, int *i, char quote_type,
	int preserve_quotes)
{
    char        *content;
    char        *result;
    int         start;
    int         len;

	input = prepare_quote_extraction(input, i, preserve_quotes, &start);
	len = 0;
	while (input[*i + len] && input[*i + len] != quote_type)
		len++;
	if (!input[*i + len])
		return (NULL);
	if (preserve_quotes)
		len++;
	if (preserve_quotes)
		content = ft_substr(input, start, len + 1);
	else
		content = ft_substr(input, start, len);
	*i += len + 1;
	if (!content)
		return (NULL);
	result = content;
	return (result);
}

char	*get_unquoted_content(char *input, int *i)
{
	int	start;
	int	len;

	start = *i;
	len = 0;
	while (input[*i + len]
		&& !is_space(input[*i + len])
		&& !is_operator(input[*i + len])
		&& input[*i + len] != '\''
		&& input[*i + len] != '"')
		len++;

	*i += len;
	return (ft_substr(input, start, len));
}

int	should_preserve_quotes(t_token *head)
{
	t_token	*current;

	if (!head)
		return (1);
	current = head;
	while (current->next)
		current = current->next;
	return (current->type == TOKEN_WORD
		&& ft_strcmp(current->value, "echo") == 0);
}
