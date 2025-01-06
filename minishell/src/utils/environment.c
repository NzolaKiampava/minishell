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

static char *create_env_var(const char *key, char *value)
{
    char    *new_var;
    char    *full_var;

    new_var = ft_strjoin(key, "=");
    if (!new_var)
        return (NULL);
    full_var = ft_strjoin(new_var, value);
    free(new_var);
    if (!full_var)
        return (NULL);
    return (full_var);
}
/*
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
*/

int set_env_value(char ***env, char *key, char *value)
{
    char    *new_var;
    int     key_len;
    int     i;

    if (!env || !key)
        return (0);
    key_len = ft_strlen(key);
    i = 0;
    // Procura pela variável existente
    while ((*env)[i])
    {
        if (ft_strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=')
        {
            // Atualiza a variável existente
            new_var = create_env_var(key, value);
            if (!new_var)
                return (0);
            free((*env)[i]);
            (*env)[i] = new_var;
            return (1);
        }
        i++;
    }
    
    // Se não encontrou, cria uma nova variável
    new_var = create_env_var(key, value);
    if (!new_var)
        return (0);
        
    // Aloca novo array com espaço adicional
    char **new_env = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_env)
    {
        free(new_var);
        return (0);
    }
    
    // Copia variáveis existentes
    int j = 0;
    while (j < i)
    {
        new_env[j] = (*env)[j];
        j++;
    }
    
    // Adiciona nova variável e NULL terminator
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    
    // Libera array antigo e atualiza ponteiro
    free(*env);
    *env = new_env;
    
    return (1);
}
