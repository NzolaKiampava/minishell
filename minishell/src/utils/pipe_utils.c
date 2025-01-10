/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-31 14:39:53 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-31 14:39:53 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipes(int curr_pipe[2], int prev_pipe[2], t_command *current)
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

void	redirect_pipes(int prev_pipe[2], int curr_pipe[2])
{
	if (prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (curr_pipe[1] != -1)
	{
		close(curr_pipe[0]);
		dup2(curr_pipe[1], STDOUT_FILENO);
		close(curr_pipe[1]);
	}
}
