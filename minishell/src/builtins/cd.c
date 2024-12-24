/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:12:05 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 20:12:05 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_tilde(char *path, t_shell *shell)
{
	char	*home;
	char	*expanded_path;

	if (path[0] != '~')
		return ft_strdup(path);

	home = get_env_value(shell->env, "HOME");
	if (!home)
		return ft_strdup(path);

	// Se path é apenas "~", retorna HOME
	if (path[1] == '\0')
		return ft_strdup(home);

	// Se path é "~/algo", concatena HOME com o resto do path
	expanded_path = ft_strjoin(home, path + 1);
	return expanded_path;
}

int ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	current_dir[1024];
	char	*old_pwd;

	// Salva o diretório atual antes de qualquer mudança
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("cd: error getting current directory");
		return EXIT_FAILURE;
	}

	// Se nenhum argumento for fornecido, vai para o diretório home
	if (!args[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			return EXIT_FAILURE;
		}
	}
	// Caso especial: cd -
	else if (ft_strcmp(args[1], "-") == 0)
	{
		old_pwd = get_env_value(shell->env, "OLDPWD");
		if (!old_pwd)
		{
			ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
			return EXIT_FAILURE;
		}
		path = old_pwd;
		// Imprime o diretório para onde está indo
		ft_putendl_fd(path, STDOUT_FILENO);
	}
	// Outros casos
	else
	{
		// Expande o til (~) se presente
		path = expand_tilde(args[1], shell);
		if (!path)
		{
			ft_putendl_fd("cd: memory allocation error", STDERR_FILENO);
			return EXIT_FAILURE;
		}
	}

	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		perror(path);
		// Libera memória se path foi alocado por expand_tilde
		if (args[1] && args[1][0] == '~')
			free(path);
		return EXIT_FAILURE;
	}

	// Libera memória se path foi alocado por expand_tilde
	if (args[1] && args[1][0] == '~')
		free(path);

	// Atualiza variável de ambiente OLDPWD
	set_env_value(&shell->env, "OLDPWD", current_dir);

	// Atualiza variável de ambiente PWD
	if (getcwd(current_dir, sizeof(current_dir)) != NULL)
	{
		set_env_value(&shell->env, "PWD", current_dir);
	}

	return EXIT_SUCCESS;
}
