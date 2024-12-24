/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:16:48 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 20:16:48 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função auxiliar para remover uma variável de ambiente
static char	**remove_env_var(char **env, int index)
{
	int	env_len = 0;
	char	**new_env;
	int	j = 0;

	// Conta o número de variáveis de ambiente
	while (env[env_len])
		env_len++;

	// Aloca novo ambiente sem a variável a ser removida
	new_env = malloc(sizeof(char *) * env_len);
	if (!new_env)
		return NULL;

	// Copia variáveis, exceto a que será removida
	for (int i = 0; i < env_len; i++)
	{
		if (i != index)
			new_env[j++] = ft_strdup(env[i]);
	}
	new_env[j] = NULL;

	// Libera o ambiente antigo
	for (int i = 0; env[i]; i++)
		free(env[i]);
	free(env);

	return new_env;
}

int	ft_unset(char **args, t_shell *shell)
{
	int i = 1;

	if (!args[1])
		return EXIT_SUCCESS;

	while (args[i])
	{
		for (int j = 0; shell->env[j]; j++)
		{
			// Verifica se o nome da variável corresponde
			if (ft_strncmp(shell->env[j], args[i], ft_strlen(args[i])) == 0 &&
			shell->env[j][ft_strlen(args[i])] == '=')
			{
				shell->env = remove_env_var(shell->env, j);
				break;
			}
		}
		i++;
	}

	return EXIT_SUCCESS;
}
