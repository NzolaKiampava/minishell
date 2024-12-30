/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-30 13:46:47 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-30 13:46:47 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_redirect_syntax(t_token *token)
{
	if (!token->next)
		return (0);
	if (token->next->type != TOKEN_WORD)
		return (0);
	if (token->next->next &&
		(token->next->next->type == TOKEN_REDIRECT_IN
			|| token->next->next->type == TOKEN_REDIRECT_OUT
			|| token->next->next->type == TOKEN_REDIRECT_APPEND
			|| token->next->next->type == TOKEN_HEREDOC))
		return (0);
	return (1);
}

int	validate_pipe_syntax(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE)
		return (0);

	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE
			&& (!tokens->next || tokens->next->type == TOKEN_PIPE))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}