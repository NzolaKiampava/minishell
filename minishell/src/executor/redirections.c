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

static int	cleanup_on_error(int saved_stdout, int saved_stdin,
	int fd, char *message)
{
	if (fd != -1)
		close(fd);
	if (saved_stdout != -1)
		close(saved_stdout);
	if (saved_stdin != -1)
		close(saved_stdin);
	if (message)
		print_error(message);
	return (0);
}

static int handle_heredoc(t_command *cmd)
{
    int pipe_fd[2];
    char *line;
    char *delimiter;
    struct sigaction sa_heredoc, sa_old;

    if (pipe(pipe_fd) == -1)
        return (-1);

    // Configura handler específico para heredoc
    sa_heredoc.sa_handler = SIG_DFL;  // Usa o handler padrão para SIGINT
    sa_heredoc.sa_flags = 0;
    sigemptyset(&sa_heredoc.sa_mask);
    sigaction(SIGINT, &sa_heredoc, &sa_old);

    delimiter = cmd->input_file;
    g_signal_received = 0;  // Reset do sinal antes de começar

    while (1)
    {
        line = readline("> ");
        // Se line é NULL (CTRL+D) ou recebeu SIGINT ou encontrou delimitador
        if (!line || g_signal_received || (line && !ft_strcmp(line, delimiter)))
        {
            free(line);
            break;
        }
        
        if (pipe_fd[1] != -1)
        {
            write(pipe_fd[1], line, ft_strlen(line));
            write(pipe_fd[1], "\n", 1);
        }
        free(line);
    }

    // Restaura o handler original
    sigaction(SIGINT, &sa_old, NULL);
    close(pipe_fd[1]);

    // Se CTRL+C foi pressionado
    if (g_signal_received)
    {
        close(pipe_fd[0]);
        return (-1);
    }

    return (pipe_fd[0]);
}


static int handle_input_redirection(t_command *cmd, int saved_stdin, int saved_stdout)
{
    if (cmd->input_fd == -2)
    {
        cmd->input_fd = handle_heredoc(cmd);
        if (cmd->input_fd == -1)
        {
            if (g_signal_received)
            {
                g_signal_received = 0;  // Reset do sinal
                return (0);
            }
            print_error("Heredoc error");
            return (cleanup_on_error(saved_stdout, saved_stdin, -1, NULL));
        }
    }
    else
        cmd->input_fd = open(cmd->input_file, O_RDONLY);

    if (cmd->input_fd == -1)
    {
        print_error("No such file or directory");
        return (cleanup_on_error(saved_stdout, saved_stdin, cmd->input_fd, NULL));
    }

    if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
        return (cleanup_on_error(saved_stdout, saved_stdin, cmd->input_fd, NULL));

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
