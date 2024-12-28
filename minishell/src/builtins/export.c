/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:18:24 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/28 10:29:18 by mazunga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_name(const char *name)
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

static char	*find_equal_sign(char *str)
{
	while (*str)
	{
		if (*str == '=')
			return (str);
		str++;
	}
	return (NULL);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	char	*name_end;

	i = 0;
	if (!args[1])
	{
		while (shell->env[i++])
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(shell->env[i], STDOUT_FILENO);
		}
		return (EXIT_SUCCESS);
	}
	i = 1;
	while (args[i++])
	{
		name_end = find_equal_sign(args[i]);
		if (name_end)
		{
			*name_end = '\0';
			if (!is_valid_name(args[i]))
			{
				ft_putstr_fd("export: `", STDERR_FILENO);
				ft_putstr_fd(args[i], STDERR_FILENO);
				ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
				return (EXIT_FAILURE);
			}
			set_env_value(&shell->env, args[i], name_end + 1);
		}
		else
		{
			if (!is_valid_name(args[i]))
			{
				ft_putstr_fd("export: `", STDERR_FILENO);
				ft_putstr_fd(args[i], STDERR_FILENO);
				ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
				return (EXIT_FAILURE);
			}
		}
	}
	return (EXIT_SUCCESS);
}
