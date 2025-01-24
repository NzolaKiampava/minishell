/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-30 14:42:41 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-30 14:42:41 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->input_fd = -1;
	cmd->output_fd = -1;
	cmd->append_mode = 0;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

static char	**allocate_new_args(t_command *cmd, int *size)
{
	char	**new_args;

	*size = 0;
	if (cmd->args)
		while (cmd->args[*size])
			*size += 1;
	new_args = (char **)malloc(sizeof(char *) * (*size + 2));
	if (!new_args)
		return (NULL);
	return (new_args);
}

int	add_arg(t_command *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		size;

	new_args = allocate_new_args(cmd, &size);
	if (!new_args)
		return (0);
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		free(new_args);
		return (0);
	}
	new_args[i + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	return (1);
}
