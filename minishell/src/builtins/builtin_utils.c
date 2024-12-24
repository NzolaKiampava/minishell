/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:59:36 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 20:59:36 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função para verificar se o comando é um built-in
int	is_builtin(char *cmd)
{
	char *builtin_commands[] = {
		"echo", 
		"cd", 
		"pwd", 
		"export", 
		"unset", 
		"env", 
		"exit",
		NULL
	};

	if (!cmd)
		return 0;

	for (int i = 0; builtin_commands[i]; i++)
	{
		if (ft_strcmp(cmd, builtin_commands[i]) == 0)
		    return 1;
	}

	return 0;
}

// Função para executar o comando built-in correto
int	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (!cmd->args[0])
		return EXIT_FAILURE;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(cmd->args));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return ft_cd(cmd->args, shell);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return ft_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return ft_export(cmd->args, shell);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return ft_unset(cmd->args, shell);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return ft_env(shell);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return ft_exit(cmd->args, shell);
	return EXIT_FAILURE;
}
