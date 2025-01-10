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

static char	*get_word(char *input, int *i, t_token *head)
{
	char	*result;
	char	*temp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i]))
	{
		temp = process_word_part(input, i, result, head);
		if (!temp)
		{
			free(result);
			return (NULL);
		}
		result = temp;
	}
	if (result[0] == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}

static int	get_operator_token(char *input, int *i, t_token **head)
{
	char				*value;
	t_token_type		type;

	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		value = ft_strdup(">>");
		type = TOKEN_REDIRECT_APPEND;
		(*i) += 2;
	}
	else if (input[*i] == '>')
	{
		value = ft_strdup(">");
		type = TOKEN_REDIRECT_OUT;
		(*i)++;
	}
	else
	{
		process_operator(input, i, &value, &type);
		if (!value)
			return (0);
	}
	add_token(head, value, type);
	return (1);
}

static int	process_token(char *input, int *i, t_token **head)
{
	char	*word;

	if (is_operator(input[*i]))
	{
		if (!get_operator_token(input, i, head))
		{
			free_tokens(*head);
			return (0);
		}
		return (1);
	}
	word = get_word(input, i, *head);
	if (!word)
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			free_tokens(*head);
			return (0);
		}
		return (1);
	}
	add_token(head, word, TOKEN_WORD);
	return (1);
}

t_token	*tokenize_input(char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		if (!process_token(input, &i, &head))
			return (NULL);
	}
	return (head);
}
