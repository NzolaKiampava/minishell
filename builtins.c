/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:21:14 by nkiampav          #+#    #+#             */
/*   Updated: 2024/11/11 15:15:09 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
			|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
			|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
			|| !ft_strcmp(cmd, "exit"));
}

int	execute_builtin(t_command *cmd)
{
	if (!ft_strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd->args));
	else if (!ft_strcmp(cmd->cmd, "cd"))
		return (ft_cd(cmd->args));
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		return (ft_pwd());
	else if (!ft_strcmp(cmd->cmd, "export"))
		return (ft_export(cmd->args));
	else if (!ft_strcmp(cmd->cmd, "unset"))
		return (ft_unset(cmd->args));
	else if (!ft_strcmp(cmd->cmd, "env"))
		return (ft_env());
	else if (!ft_strcmp(cmd->cmd, "exit"))
		return (ft_exit(cmd->args));
	return (1);
}

int	ft_echo(char **args)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 1;

	// Check for -n option
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		new_line = 0;
		i++;
	}

	// print arguments
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}

	if (new_line)
		ft_putchar_fd('\n', 1);
	return (0);
}

int	ft_cd(char **args)
{
	char	*path;

	// No arguments: got to HOME
	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putendl_fd("minshell: cd: HOME not set", 2);
			return (1);
		}
	}
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	return (0);
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}

int	ft_env(void)
{
	extern char	**environ;
	int	i;

	i = 0;
	while (environ[i])
	{
		ft_putendl_fd(environ[i], 1);
		i++;
	}
	return (0);
}

int	ft_exit(char **args)
{
	int	exit_code;

	if (!args[1])
		exit(0);
	exit_code = ft_atoi(args[i]);
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	exit(exit_code);
}

// Note: export and unset require environment variable management
// Here's a basic implementation:

int	ft_export(char **args)
{
	int	i;
	char	*equals;

	if (!args[1])
	{
		// Print sorted environment (not implement here)
		return (ft_env());
	}

	i = 1;
	while (args[i])
	{
		equals = ft_strchr(args[i], '=');
		if (equals)
		{
			*equals = '\0';
			if (setenv(args[i], equals + 1, 1) == -1)
			{
				ft_putendl_fd("minishell: export: error setting variable", 2);
				return (1);
			}
			*equals = '=';
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (unsetenv(args[i]) == -1)
		{
			ft_putendl_fd("minishell: unset: error unsetting variable", 2);
			return (1);
		}
		i++;
	}
	return (0);
}
