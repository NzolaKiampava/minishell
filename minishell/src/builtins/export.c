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

static void	sort_env_array(char **arr, int size)
{
	int		i;
	int		j;
	char	*temp;

	i = -1;
	while (++i < size - 1)
	{
		j = -1;
		while (++j < size - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

static int	print_env_vars(t_shell *shell)
{
	char	**sorted_env;
	int		env_size;
	int		i;

	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	sorted_env = ft_calloc(env_size + 1, sizeof(char *));
	if (!sorted_env)
		return (EXIT_FAILURE);
	i = -1;
	while (++i < env_size)
		sorted_env[i] = shell->env[i];
	sort_env_array(sorted_env, env_size);
	i = -1;
	while (++i < env_size)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(sorted_env[i], STDOUT_FILENO);
	}
	free(sorted_env);
	return (EXIT_SUCCESS);
}
/*
static int	handle_equal_case(char *arg, char *equal_pos, t_shell *shell)
{
	char	equal_char;
	int		status;

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
	return (EXIT_SUCCESS);
}
*/
static int process_export_arg(char *arg, t_shell *shell)
{
    char    *equal_pos;
    char    equal_char;
    int     status;

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
        status = set_env_value(&shell->env, arg, NULL); // Passa NULL quando não há '='
    
    return (EXIT_SUCCESS);
}


int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	exit_status;

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
