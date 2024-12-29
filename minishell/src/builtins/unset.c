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

static void	free_env_array(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

static char	**remove_env_var(char **env, int index)
{
	char		**new_env;
	int			env_len;
	int			j;
	int			i;

	env_len = 0;
	while (env[env_len])
		env_len++;
	new_env = malloc(sizeof(char *) * env_len);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (i < env_len)
	{
		if (i != index)
			new_env[j++] = ft_strdup(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free_env_array(env);
	return (new_env);
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	j;

	i = 1;
	if (!args[1])
		return (EXIT_SUCCESS);
	while (args[i])
	{
		j = 0;
		while (shell->env[j])
		{
			if (ft_strncmp(shell->env[j], args[i], ft_strlen(args[i])) == 0
				&& shell->env[j][ft_strlen(args[i])] == '=')
			{
				shell->env = remove_env_var(shell->env, j);
				break ;
			}
			j++;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
