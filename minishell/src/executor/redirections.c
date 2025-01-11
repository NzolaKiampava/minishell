/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:39:08 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 10:39:12 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc(t_command *cmd)
{
	struct sigaction		sa_heredoc;
	struct sigaction		sa_old;
	int						pipe_fd[2];
	int						signal_received;

	if (pipe(pipe_fd) == -1)
		return (-1);
	sa_heredoc.sa_handler = SIG_DFL;
	sa_heredoc.sa_flags = 0;
	sigemptyset(&sa_heredoc.sa_mask);
	sigaction(SIGINT, &sa_heredoc, &sa_old);
	g_signal_received = 0;
	signal_received = write_heredoc_content(pipe_fd, cmd->input_file);
	sigaction(SIGINT, &sa_old, NULL);
	close(pipe_fd[1]);
	if (signal_received)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}

static int	handle_heredoc_input(t_command *cmd, int saved_stdin,
	int saved_stdout)
{
	cmd->input_fd = handle_heredoc(cmd);
	if (cmd->input_fd == -1)
	{
		if (g_signal_received)
		{
			g_signal_received = 0;
			return (0);
		}
		print_error("Heredoc error");
		return (cleanup_on_error(saved_stdout, saved_stdin, -1, NULL));
	}
	return (1);
}

static int	handle_input_redirection(t_command *cmd, int saved_stdin,
	int saved_stdout)
{
	int	status;

	if (cmd->input_fd == -2)
	{
		status = handle_heredoc_input(cmd, saved_stdin, saved_stdout);
		if (status <= 0)
			return (status);
	}
	else
		cmd->input_fd = open(cmd->input_file, O_RDONLY);
	if (cmd->input_fd == -1)
	{
		print_error("No such file or directory");
		return (cleanup_on_error(saved_stdout, saved_stdin,
				cmd->input_fd, NULL));
	}
	if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
		return (cleanup_on_error(saved_stdout, saved_stdin,
				cmd->input_fd, NULL));
	close(cmd->input_fd);
	return (1);
}

static int	handle_output_redirection(t_command *cmd,
	int saved_stdin, int saved_stdout)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	cmd->output_fd = open(cmd->output_file, flags, 0644);
	if (cmd->output_fd == -1)
	{
		print_error("Cannot open output file");
		if (cmd->input_file)
			dup2(saved_stdin, STDIN_FILENO);
		return (cleanup_on_error(saved_stdout, saved_stdin, -1, NULL));
	}
	if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
	{
		print_error("Failed to redirect output");
		return (cleanup_on_error(saved_stdout, saved_stdin,
				cmd->output_fd, NULL));
	}
	close(cmd->output_fd);
	return (1);
}

int	handle_redirections(t_command *cmd)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdout == -1 || saved_stdin == -1)
		return (cleanup_on_error(saved_stdout, saved_stdin, -1, NULL));
	if (cmd->input_file
		&& !handle_input_redirection(cmd, saved_stdin, saved_stdout))
		return (0);
	if (cmd->output_file
		&& !handle_output_redirection(cmd, saved_stdin, saved_stdout))
		return (0);
	close(saved_stdout);
	close(saved_stdin);
	return (1);
}
