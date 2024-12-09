/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:18:24 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 20:18:24 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função auxiliar para verificar se um nome de variável é válido
static int is_valid_name(const char *name)
{
    if (!name || !*name || (!ft_isalpha(*name) && *name != '_'))
        return 0;

    while (*name)
    {
        if (!ft_isalnum(*name) && *name != '_')
            return 0;
        name++;
    }

    return 1;
}

// Função auxiliar para encontrar a posição de '=' em uma string
static char *find_equal_sign(char *str)
{
    while (*str)
    {
        if (*str == '=')
            return str;
        str++;
    }
    return NULL;
}

int ft_export(char **args, t_shell *shell)
{
    if (!args[1])
    {
        // Se nenhum argumento, imprimir todas as variáveis de ambiente
        for (int i = 0; shell->env[i]; i++)
        {
            ft_putstr_fd("declare -x ", STDOUT_FILENO);
            ft_putendl_fd(shell->env[i], STDOUT_FILENO);
        }
        return EXIT_SUCCESS;
    }

    for (int i = 1; args[i]; i++)
    {
        char *name_end = find_equal_sign(args[i]);
        
        if (name_end)
        {
            // Exportando variável com valor
            *name_end = '\0';  // Separa nome e valor
            
            if (!is_valid_name(args[i]))
            {
                ft_putstr_fd("export: `", STDERR_FILENO);
                ft_putstr_fd(args[i], STDERR_FILENO);
                ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
                return EXIT_FAILURE;
            }
            
            set_env_value(&shell->env, args[i], name_end + 1);
        }
        else
        {
            // Nome de variável sem valor
            if (!is_valid_name(args[i]))
            {
                ft_putstr_fd("export: `", STDERR_FILENO);
                ft_putstr_fd(args[i], STDERR_FILENO);
                ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}