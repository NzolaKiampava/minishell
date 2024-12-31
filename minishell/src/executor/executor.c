/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:41:31 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 12:41:33 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command_path(char *cmd, char **env)
{
	char	*path;
	char	**paths;

	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	return (search_in_path(paths, cmd));
}

static int	run_builtin_command(t_command *cmd, t_shell *shell,
	int saved_stdout, int saved_stdin)
{
	int	status;

	if (!handle_redirections(cmd))
	{
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		return (1);
	}
	status = execute_builtin(cmd, shell);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
	return (status);
}

static void	execute_child(t_command *cmd, t_shell *shell)
{
	char	*path;

	if (!handle_redirections(cmd))
		exit(1);
	path = get_command_path(cmd->args[0], shell->env);
	if (!path)
	{
		print_error("Command not found");
		exit(127);
	}
	execve(path, cmd->args, shell->env);
	free(path);
	exit(1);
}

static int	execute_single_command(t_command *cmd, t_shell *shell)
{
	int			status;
	int			saved_stdout;
	int			saved_stdin;
	pid_t		pid;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (is_builtin(cmd->args[0]))
		return (run_builtin_command(cmd, shell, saved_stdout, saved_stdin));
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		execute_child(cmd, shell);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_commands(t_shell *shell)
{
	int						has_pipe;
	t_command				*cmd;
	t_command				*tmp;

	cmd = shell->commands;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	has_pipe = 0;
	tmp = cmd;
	while (tmp->next)
	{
		has_pipe = 1;
		tmp = tmp->next;
	}
	if (has_pipe)
		return (execute_piped_commands(cmd, shell));
	else
		return (execute_single_command(cmd, shell));
}
