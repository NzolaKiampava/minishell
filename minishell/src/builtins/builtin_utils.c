/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:59:36 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/27 21:17:55 by mazunga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	int			i;
	char		*builtin_commands[8];

	builtin_commands[0] = "echo";
	builtin_commands[1] = "cd";
	builtin_commands[2] = "pwd";
	builtin_commands[3] = "export";
	builtin_commands[4] = "unset";
	builtin_commands[5] = "env";
	builtin_commands[6] = "exit";
	builtin_commands[7] = NULL;
	if (!cmd)
		return (0);
	i = 0;
	while (builtin_commands[i])
	{
		if (ft_strcmp(cmd, builtin_commands[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (!cmd->args[0])
		return (EXIT_FAILURE);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(cmd->args));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(cmd->args, shell));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(cmd->args, shell));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(cmd->args, shell));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(shell));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(cmd->args, shell));
	return (EXIT_FAILURE);
}
