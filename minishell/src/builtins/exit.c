/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:11:12 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/27 21:52:41 by mazunga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **args, t_shell *shell)
{
	long	status;
	char	*endptr;

	shell->running = 0;
	status = 0;
	if (args[1])
	{
		status = ft_strtol(args[1], &endptr, 10);
		if (*endptr != '\0')
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			status = 255;
		}
		else if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			shell->running = 1;
			return (EXIT_FAILURE);
		}
	}
	ft_putendl_fd("exit", STDERR_FILENO);
	exit((int)status);
}
