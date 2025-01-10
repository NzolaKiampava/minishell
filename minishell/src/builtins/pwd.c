/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:08:44 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 20:08:44 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	*current_path;

	current_path = getcwd(NULL, 0);
	if (current_path == NULL)
	{
		perror("pwd: error retrieving current directory");
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(current_path, STDOUT_FILENO);
	free(current_path);
	return (EXIT_SUCCESS);
}
