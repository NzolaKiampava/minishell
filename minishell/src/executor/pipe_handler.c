/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:28:04 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 10:28:06 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipes(int curr_pipe[2], int prev_pipe[2], t_command *current)
{
	if (current->next)
	{
		if (pipe(curr_pipe) == -1)
		{
			print_error("pipe error");
			if (prev_pipe[0] != -1)
			{
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			return (0);
		}
	}
	return (1);
}

static void	handle_child_process(int prev_pipe[2], int curr_pipe[2],
		t_command *cmd, t_shell *shell)
{
	char	*path;

	if (prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (cmd->next)
	{
		close(curr_pipe[0]);
		dup2(curr_pipe[1], STDOUT_FILENO);
		close(curr_pipe[1]);
	}
	if (!handle_redirections(cmd))
		exit(1);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	path = get_command_path(cmd->args[0], shell->env);
	if (!path)
	{
		print_error("Command not found");
		exit(127);
	}
	execve(path, cmd->args, shell->env);
	free(path);
	perror("execve failed");
	exit(1);
}

static int	fork_and_execute(t_command *current, t_shell *shell,
		int prev_pipe[2], int curr_pipe[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		print_error("fork error");
		cleanup_pipes(prev_pipe, curr_pipe, current->next != NULL);
		return (-1);
	}
	if (pid == 0)
		handle_child_process(prev_pipe, curr_pipe, current, shell);
	return (pid);
}

int	execute_piped_commands(t_command *cmd, t_shell *shell)
{
	int			prev_pipe[2];
	int			curr_pipe[2];
	int			status;
	int			last_status;
	t_command	*current;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	curr_pipe[0] = -1;
	curr_pipe[1] = -1;
	last_status = 0;
	current = cmd;
	while (current)
	{
		if (!setup_pipes(curr_pipe, prev_pipe, current))
			return (1);
		if (fork_and_execute(current, shell, prev_pipe, curr_pipe) == -1)
			return (1);
		cleanup_pipes(prev_pipe, curr_pipe, 0);
		if (current->next)
		{
			prev_pipe[0] = curr_pipe[0];
			prev_pipe[1] = curr_pipe[1];
		}
		current = current->next;
	}
	while (wait(&status) > 0)
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
	return (last_status);
}
