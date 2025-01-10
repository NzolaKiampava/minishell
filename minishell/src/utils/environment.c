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
	full_var = ft_strjoin(new_var, value);
	free(new_var);
	if (!full_var)
		return (NULL);
	return (full_var);
}

static int	update_existing_env(char **env, char *key, char *value, int key_len)
{
	char		*new_var;
	int			i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			new_var = create_env_var(key, value);
			if (!new_var)
				return (0);
			free(env[i]);
			env[i] = new_var;
			return (1);
		}
		i++;
	}
	return (-1);
}

int	set_env_value(char ***env, char *key, char *value)
{
	char		*new_var;
	int			status;

	if (!env || !key)
		return (0);
	status = update_existing_env(*env, key, value, ft_strlen(key));
	if (status >= 0)
		return (status);
	new_var = create_env_var(key, value);
	if (!new_var)
		return (0);
	return (add_new_env_var(env, new_var));
}
