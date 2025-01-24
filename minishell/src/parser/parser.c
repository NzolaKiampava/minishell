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

static int	handle_redirect_type(t_command *cmd, t_token_type type, char *value)
{
	if (type == TOKEN_REDIRECT_IN)
	{
		if (cmd->input_file)
			free(cmd->input_file);
		cmd->input_file = ft_strdup(value);
		cmd->input_fd = -1;
	}
	else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND)
	{
		if (cmd->output_file)
			free(cmd->output_file);
		cmd->output_file = ft_strdup(value);
		cmd->output_fd = -1;
		cmd->append_mode = (type == TOKEN_REDIRECT_APPEND);
	}
	else if (type == TOKEN_HEREDOC)
	{
		if (cmd->input_file)
			free(cmd->input_file);
		cmd->input_file = ft_strdup(value);
		cmd->input_fd = -2;
	}
	return (1);
}

static int	handle_redirect(t_command *cmd, t_token **token)
{
	t_command	*current_cmd;
	t_command	*new_cmd;

	if (!validate_redirect_syntax(*token))
	{
		print_error("syntax error near unexpected token");
		return (0);
	}
	current_cmd = cmd;
	while (current_cmd->next)
		current_cmd = current_cmd->next;
	if ((*token)->type == TOKEN_REDIRECT_OUT)
	{
		new_cmd = create_command();
		if (!new_cmd)
			return (0);
		current_cmd->next = new_cmd;
		new_cmd->prev = current_cmd;
		current_cmd = new_cmd;
	}
	if (!handle_redirect_type(current_cmd,
			(*token)->type, (*token)->next->value))
		return (0);
	*token = (*token)->next->next;
	return (1);
}

static	t_command	*handle_command_creation(t_command **head,
	t_command **current, t_token **token)
{
	if (!*head)
	{
		*head = create_command();
		*current = *head;
		if (!*current)
			return (NULL);
	}
	else if ((*token)->type == TOKEN_PIPE)
	{
		if (!(*current)->args)
		{
			print_error("syntax error near unexpected token '|'");
			return (NULL);
		}
		(*current)->next = create_command();
		if (!(*current)->next)
			return (NULL);
		*current = (*current)->next;
		*token = (*token)->next;
	}
	return (*head);
}

static bool	handle_token_types(t_command *current, t_token **token)
{
	if ((*token)->type == TOKEN_WORD)
	{
		if (!add_arg(current, (*token)->value))
			return (false);
		*token = (*token)->next;
	}
	else if ((*token)->type >= TOKEN_REDIRECT_IN
		&& (*token)->type <= TOKEN_HEREDOC)
	{
		if (!handle_redirect(current, token))
			return (false);
	}
	else
		*token = (*token)->next;
	return (true);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_token		*token;

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
		if (!handle_command_creation(&head, &current, &token))
			return (NULL);
		if (token && token->type == TOKEN_PIPE)
			continue ;
		if (!handle_token_types(current, &token))
		{
			free_commands(head);
			return (NULL);
		}
	}
	return (head);
}
