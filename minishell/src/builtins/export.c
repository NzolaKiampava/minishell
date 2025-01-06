/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:18:24 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-28 10:29:18 by mazunga          ###   ########.fr       */
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
			break;
		}
		str++;
	}
	return (equal_pos);
}

static int	print_env_vars(t_shell *shell)
{
	int		i;
	char	**sorted_env;
	int		env_size;

	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	sorted_env = ft_calloc(env_size + 1, sizeof(char *));
	if (!sorted_env)
		return (EXIT_FAILURE);
	i = -1;
	while (++i < env_size)
		sorted_env[i] = shell->env[i];
	i = -1;
	while (++i < env_size - 1)
	{
		int j = -1;
		while (++j < env_size - i - 1)
			if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
			{
				char *temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
	}
	i = -1;
	while (++i < env_size)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(sorted_env[i], STDOUT_FILENO);
	}
	free(sorted_env);
	return (EXIT_SUCCESS);
}

static int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (EXIT_FAILURE);
}

static int	process_export_arg(char *arg, t_shell *shell)
{
	char	*equal_pos;
	char	equal_char;
	int		status;

	equal_pos = find_equal_sign(arg);
	if (equal_pos)
	{
		equal_char = *equal_pos;
		*equal_pos = '\0';
		
		if (!is_valid_name(arg))
		{
			*equal_pos = equal_char;
			return (handle_invalid_identifier(arg));
		}
		
		status = set_env_value(&shell->env, arg, equal_pos + 1);
		*equal_pos = equal_char;
		
		if (!status)
		{
			ft_putstr_fd("minishell: export: memory allocation error\n", 
				STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	else if (!is_valid_name(arg))
		return (handle_invalid_identifier(arg));
	else
		status = set_env_value(&shell->env, arg, "");
	
	return (EXIT_SUCCESS);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	int		exit_status;

	if (!args[1])
		return (print_env_vars(shell));

	exit_status = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], shell) == EXIT_FAILURE)
			exit_status = EXIT_FAILURE;
		i++;
	}

	return (exit_status);
}
