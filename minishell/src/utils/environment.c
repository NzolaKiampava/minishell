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

static char	*create_env_var(const char *key, const char *value)
{
	char	*new_var;
	size_t	total_len;

	if (!value)
		return (ft_strdup(key));
	total_len = ft_strlen(key) + ft_strlen(value) + 2;
	new_var = (char *)malloc(total_len);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, key, total_len);
	ft_strlcat(new_var, "=", total_len);
	ft_strlcat(new_var, value, total_len);
	return (new_var);
}

static int	update_existing_env(char **env, char *key, char *value, int key_len)
{
	char	*new_var;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0
			&& (env[i][key_len] == '=' || env[i][key_len] == '\0'))
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

static int	add_new_env(char ***env, char *key, char *value, int env_size)
{
	char	**new_env;
	char	*new_var;
	int		i;

	new_var = create_env_var(key, value);
	if (!new_var)
		return (0);
	new_env = (char **)malloc(sizeof(char *) * (env_size + 2));
	if (!new_env)
	{
		free(new_var);
		return (0);
	}
	i = 0;
	while (i < env_size)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[env_size] = new_var;
	new_env[env_size + 1] = NULL;
	free(*env);
	*env = new_env;
	return (1);
}

int	set_env_value(char ***env, char *key, char *value)
{
	int	key_len;
	int	update_status;
	int	env_size;

	if (!env || !key)
		return (0);
	key_len = ft_strlen(key);
	update_status = update_existing_env(*env, key, value, key_len);
	if (update_status >= 0)
		return (update_status);
	env_size = 0;
	while ((*env)[env_size])
		env_size++;
	return (add_new_env(env, key, value, env_size));
}
