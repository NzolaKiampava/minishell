/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 09:31:34 by nkiampav          #+#    #+#             */
/*   Updated: 2025/01/09 09:32:07 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_name(const char *name)
{
	if (!name || !*name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

char	*find_equal_sign(char *str)
{
	char	*equal_pos;
	char	in_quotes;

	if (!str)
		return (NULL);
	equal_pos = NULL;
	in_quotes = 0;
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			if (!in_quotes)
				in_quotes = *str;
			else if (in_quotes == *str)
				in_quotes = 0;
		}
		else if (*str == '=' && !in_quotes)
		{
			equal_pos = str;
			break ;
		}
		str++;
	}
	return (equal_pos);
}

int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (EXIT_FAILURE);
}

int	add_new_env_var(char ***env, char *new_var)
{
	char			**new_env;
	int				i;

	i = 0;
	while ((*env)[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		free(new_var);
		return (0);
	}
	ft_memcpy(new_env, *env, sizeof(char *) * i);
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
	return (1);
}
