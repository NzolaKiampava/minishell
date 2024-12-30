/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-30 16:52:05 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-30 16:52:05 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_word_parts(char *result, char *next_part)
{
	char	*temp;
	char	*final_result;

	temp = result;
	final_result = ft_strjoin(result, next_part);
	free(temp);
	free(next_part);
	return (final_result);
}

char	*process_word_part(char *input, int *i, char *result,
	t_token *head)
{
    char    *next_part;
    char    quote_type;
    int     preserve_quotes;

    preserve_quotes = should_preserve_quotes(head);
    if (input[*i] == '\'' || input[*i] == '"')
    {
        quote_type = input[*i];
        next_part = get_content_between_quotes(input, i, quote_type,
                                                    preserve_quotes);
    }
    else
        next_part = get_unquoted_content(input, i);
    if (!next_part)
    {
        free(result);
        return (NULL);
    }
    return (join_word_parts(result, next_part));
}

void	process_operator(char *input, int *i, char **value,
	t_token_type *type)
{
	if (input[*i] == '|')
	{
		*value = ft_strdup("|");
		*type = TOKEN_PIPE;
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			*value = ft_strdup("<<");
			*type = TOKEN_HEREDOC;
			(*i) += 2;
		}
		else
		{
			*value = ft_strdup("<");
			*type = TOKEN_REDIRECT_IN;
			(*i)++;
		}
	}
}