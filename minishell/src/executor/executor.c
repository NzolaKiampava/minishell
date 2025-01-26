/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:41:31 by nkiampav          #+#    #+#             */
/*   Updated: 2025/01/09 15:36:18 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_builtin_command(t_command *cmd, t_shell *shell,
	int saved_stdout, int saved_stdin)
{
	int	status;

	if (!handle_redirections(cmd, shell))
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

	if (cmd->append_mode)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	if (!handle_redirections(cmd, shell))
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

static int	execute_external_command(t_command *cmd, t_shell *shell)
{
	struct sigaction		sa_old;
	pid_t					pid;
	int						status;

	if (cmd->append_mode)
	{
		sigaction(SIGINT, NULL, &sa_old);
		signal(SIGINT, SIG_IGN);
	}
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		execute_child(cmd, shell);
	waitpid(pid, &status, 0);
	if (cmd->append_mode)
		sigaction(SIGINT, &sa_old, NULL);
	return (handle_signal_status(status));
}

static int	execute_single_command(t_command *cmd, t_shell *shell)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (is_builtin(cmd->args[0]))
		return (run_builtin_command(cmd, shell, saved_stdout, saved_stdin));
	return (execute_external_command(cmd, shell));
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
