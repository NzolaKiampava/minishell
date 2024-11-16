/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:50:41 by nkiampav          #+#    #+#             */
/*   Updated: 2024/11/11 16:10:01 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_command_path(char *cmd)
{
	char	*path;
	char	**paths;
	char	*full_path;
	int	i;

	// If commands contains '/', treat as direct path
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));

	// get PATH environment variable
	path = getenv("PATH");
	if (!path)
		return (NULL);

	// Split PATH into individual directories
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);

	// Try each directory
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, cmd);

		if (access(full_path, X_OK) == 0)
		{
			// Free paths array
			i = 0;
			while (paths[i])
				free(paths[i++]);
			free(paths);
			return (full_path);
		}

		free(full_path);
		i++;
	}

	// Free paths array
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);

	return (NULL);
}

int	execute_command(t_command *cmd)
{
	char	*cmd_path;
	pid_t	pid;
	int	status;

	if (!cmd || !cmd->cmd)
		return (1);

	// Check if it's a builtin command
	if (is_builtin(cmd->cmd))
		return (execute_builtin(cmd));

	// Find command in PATH
	cmd_path = find_command_path(cmd->cmd);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found", 2);
		ft_putstr_fd(cmd->cmd, 2);
		return (127);
	}


	// Fork and execute
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (1);
	}

	if (pid == 0)
	{
		// Child process
		if (execve(cmd_path, cmd->args, environ))
		{
			perror("minishell");
			exit(126);
		}
	}
	else
	{
		// Parent process
		waitpid(pid, &status, 0);
		free(cmd_path);

		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
	return (0);
}
