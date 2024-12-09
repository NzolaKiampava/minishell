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

int ft_cd(char **args, t_shell *shell)
{
    char *path;
    char current_dir[1024];

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
    else
    {
        path = args[1];
    }

    // Salva o diretório atual antes de mudar
    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        perror("cd: error getting current directory");
        return EXIT_FAILURE;
    }

    if (chdir(path) != 0)
    {
        ft_putstr_fd("cd: ", STDERR_FILENO);
        perror(path);
        return EXIT_FAILURE;
    }

    // Atualiza variável de ambiente OLDPWD
    set_env_value(&shell->env, "OLDPWD", current_dir);

    // Atualiza variável de ambiente PWD
    if (getcwd(current_dir, sizeof(current_dir)) != NULL)
    {
        set_env_value(&shell->env, "PWD", current_dir);
    }

    return EXIT_SUCCESS;
}