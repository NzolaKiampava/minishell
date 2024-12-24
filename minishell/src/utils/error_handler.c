/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-05 21:26:56 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-05 21:26:56 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(msg, 2);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

void	free_commands(t_command *commands)
{
	t_command *temp;
	int i;

	while (commands)
	{
		temp = commands;
		commands = commands->next;

		if (temp->args)
		{
			i = 0;
			while (temp->args[i])
				free(temp->args[i++]);
			free(temp->args);
		}

		if (temp->input_file)
			free(temp->input_file);
		if (temp->output_file)
			free(temp->output_file);

		free(temp);
	}
}

void	free_shell(t_shell *shell)
{
	int	i;

	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
			free(shell->env[i++]);
		free(shell->env);
	}

	if (shell->commands)
		free_commands(shell->commands);

	if (shell->tokens)
		free_tokens(shell->tokens);
}
