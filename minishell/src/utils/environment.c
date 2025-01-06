/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-05 21:29:21 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-05 21:29:21 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_env(char **env)
{
	char	**new_env;
	int		i;
	int		size;

	size = 0;
	while (env[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	*get_env_value(char **env, char *key)
{
	int	i;
	int	key_len;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

static char	*create_env_var(const char *key, char *value)
{
	char	*new_var;
	char	*full_var;

	new_var = ft_strjoin(key, "=");
	if (!new_var)
		return (NULL);
	full_var = ft_strjoin_free(new_var, value);
	return (full_var);
}

static char	**allocate_new_env(char **env, int size, char *new_var)
{
	char		**new_env;
	int			i;

	new_env = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[size] = new_var;
	new_env[size + 1] = NULL;
	return (new_env);
}

int	set_env_value(char ***env, char *key, char *value)
{
	char		*new_var;
	char		**new_env;
	int			size;
	int			i;

	new_var = create_env_var(key, value);
	if (!new_var)
		return (0);
	size = 0;
	while ((*env)[size])
		size++;
	new_env = allocate_new_env(*env, size, new_var);
	if (!new_env)
	{
		free(new_var);
		return (0);
	}
	i = -1;
	while ((*env)[++i])
		free((*env)[i]);
	free(*env);
	*env = new_env;
	return (1);
}
