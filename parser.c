/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:25:23 by nkiampav          #+#    #+#             */
/*   Updated: 2024/11/11 15:50:32 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	count_args(char *input)
{
	int	count;
	int	in_quotes;
	int	quote_char;

	count = 0;
	in_quotes = 0;
	quote_char = 0;

	while (*input)
	{
		// Skip spaces unless in quotes
		while (is_space(*input) && !in_quotes)
			input++;
		if (!*input)
			break;

		// Handle quotes
		if (*input == '"' || *input == '\'')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = *input;
			}
			else if (*input == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
			}
		}

		// Count argument
		if (!is_space(*input) && (!in_quotes || *input == quote_char))
			count++;

		// Skip to next space or end
		while (*input && (!is_space(*input) || in_quotes))
			input++;
	}
	return (count);
}

static char	*extract_arg(char **input)
{
	char	*start;
	char	*arg;
	int	len = 0;
	int	in_quotes = 0;
	char	quote_char = 0;

	// Skip leading spaces
	while (is_space(**input))
		(*input)++;
	start = *input;

	// Calculate argument length
	while (**input && (!is_space(**input) || in_quotes))
	{
		if (**input == '"' || **input == '\'')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = **input;
			}
			else if (**input == quote_char)
			{
				in_quote = 0;
				quote_char = 0;
			}
		}
		len++;
		(*input)++;
	}

	// Allocate and copy argument
	arg = (char *)malloc(sizeof(char) * (len + 1));
	if (!arg)
		return (NULL);
	ft_strlcpy(arg, start, len + 1);
	return (arg);
}

int	parse_input(char *input, t_command **cmd)
{
	t_command	*new_cmd;
	int	i;
	int	arg_count;

	// Initialize new command
	new_cmd = (t_command *)malloc(sizeof(t_command));
	if (!new_cmd)
		return (1);
	ft_memset(new_cmd, 0, sizeof(t_command));

	// COunt arguments and allocate array
	arg_count = count_arg(input);
	new_cmd->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!new_cmd->args)
	{
		free(new_cmd);
		return (1);
	}

	// Extract arguments
	i = 0;
	while (i < arg_count)
	{
		new_cmd->args[i] = extract_arg(&input);
		if (!new_cmd->args[i])
		{
			// Handle error: free everything and return
			while (--i >= 0)
				free(new_cmd->args[i]);
			free(new_cmd->args);
			free(new_cmd);
			return (1);
		}
		i++;
	}
	new_cmd->args[i] = NULL;

	// Set command (first argument)
	if (arg_count > 0)
		new_cmd->cmd = ft_strdup(new_cmd->args[0]);
	*cmd = new_cmd;
	return (0);
}
