/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:12:05 by maalmeid          #+#    #+#             */
/*   Updated: 2024/12/27 21:36:38 by mazunga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_tilde(char *path, t_shell *shell)
{
	char	*home;
	char	*expanded_path;

	if (path[0] != '~')
		return (ft_strdup(path));
	home = get_env_value(shell->env, "HOME");
	if (!home)
		return (ft_strdup(path));
	if (path[1] == '\0')
		return (ft_strdup(home));
	expanded_path = ft_strjoin(home, path + 1);
	return (expanded_path);
}

static char	*get_home_or_oldpwd(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
	}
	else
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
			return (NULL);
		}
		ft_putendl_fd(path, STDOUT_FILENO);
	}
	return (path);
}

static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1] || ft_strcmp(args[1], "-") == 0)
		return (get_home_or_oldpwd(args, shell));
	path = expand_tilde(args[1], shell);
	if (!path)
	{
		ft_putendl_fd("cd: memory allocation error", STDERR_FILENO);
		return (NULL);
	}
	return (path);
}

static int	update_directory(char *path, char **args,
								t_shell *shell, char *old_dir)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		perror(path);
		if (args[1] && args[1][0] == '~')
			free(path);
		return (EXIT_FAILURE);
	}
	if (args[1] && args[1][0] == '~')
		free(path);
	set_env_value(&shell->env, "OLDPWD", old_dir);
	if (getcwd(old_dir, 1024) != NULL)
		set_env_value(&shell->env, "PWD", old_dir);
	return (EXIT_SUCCESS);
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	current_dir[1024];

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("cd: error getting current directory");
		return (EXIT_FAILURE);
	}
	path = get_cd_path(args, shell);
	if (!path)
		return (EXIT_FAILURE);
	return (update_directory(path, args, shell, current_dir));
}
