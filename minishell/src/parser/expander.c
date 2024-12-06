/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:16:56 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 11:16:57 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_variable_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

static char *get_variable_name(char *str)
{
    int     i;
    char    *name;

    i = 0;
    while (str[i] && is_variable_char(str[i]))
        i++;
    
    name = (char *)malloc(sizeof(char) * (i + 1));
    if (!name)
        return (NULL);
    ft_strlcpy(name, str, i + 1);
    return (name);
}

static char *expand_exit_status(t_shell *shell)
{
    return (ft_itoa(shell->exit_status));
}

static char *handle_special_var(char c, t_shell *shell)
{
    if (c == '?')
        return expand_exit_status(shell);
    // Add other special variables here if needed ($0, $1, etc.)
    return (ft_strdup(""));
}

static char *expand_single_var(char *str, int *i, t_shell *shell)
{
    char *var_name;
    char *var_value;
    
    (*i)++;  // Skip the $
    
    // Handle special variables ($?, $0, etc.)
    if (!is_variable_char(str[*i]))
    {
        char *value = handle_special_var(str[*i], shell);
        if (str[*i])
            (*i)++;
        return value;
    }

    // Handle regular variables
    var_name = get_variable_name(str + *i);
    if (!var_name)
        return (NULL);

    while (str[*i] && is_variable_char(str[*i]))
        (*i)++;

    var_value = get_env_value(shell->env, var_name);
    free(var_name);

    return (var_value ? ft_strdup(var_value) : ft_strdup(""));
}

static char *expand_variables_in_str(char *str, t_shell *shell)
{
    int     i;
    char    *result;
    char    *temp;
    char    *var_value;
    int     in_single_quotes;

    result = ft_strdup("");
    i = 0;
    in_single_quotes = 0;

    while (str[i])
    {
        if (str[i] == '\'')
            in_single_quotes = !in_single_quotes;
        
        if (str[i] == '$' && !in_single_quotes)
        {
            var_value = expand_single_var(str, &i, shell);
            temp = result;
            result = ft_strjoin(result, var_value);
            free(temp);
            free(var_value);
            continue;
        }

        temp = result;
        result = ft_strjoin_char(result, str[i]);
        free(temp);
        i++;
    }

    return (result);
}

void expand_variables(t_command *cmd, t_shell *shell)
{
    int i;
    char *expanded;

    if (!cmd || !cmd->args)
        return;

    i = 0;
    while (cmd->args[i])
    {
        expanded = expand_variables_in_str(cmd->args[i], shell);
        free(cmd->args[i]);
        cmd->args[i] = expanded;
        i++;
    }

    if (cmd->input_file)
    {
        expanded = expand_variables_in_str(cmd->input_file, shell);
        free(cmd->input_file);
        cmd->input_file = expanded;
    }

    if (cmd->output_file)
    {
        expanded = expand_variables_in_str(cmd->output_file, shell);
        free(cmd->output_file);
        cmd->output_file = expanded;
    }
}
