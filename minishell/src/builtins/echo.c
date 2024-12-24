/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:07:32 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 20:07:32 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int i = 1;
	int no_newline = 0;

	// Verifica se a flag -n está presente
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
	{
		no_newline = 1;
		i++;
	}

	// Imprime os argumentos
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));

		// Adiciona espaço entre argumentos, exceto no último
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}

	// Adiciona nova linha, a menos que -n seja especificado
	if (!no_newline)
		write(STDOUT_FILENO, "\n", 1);
	return EXIT_SUCCESS;
}
