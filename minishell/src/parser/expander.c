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

static char *expand_single_var(char *str, int *i, t_shell *shell)
{
    int start;
    int len;
    char *var_name;
    char *var_value;
    char *result;

    (*i)++; // Skip the '$'
    start = *i;
    len = 0;

    // Handle special case $?
    if (str[*i] == '?')
    {
        (*i)++;
        return (ft_itoa(shell->exit_status));
    }

    // Get variable name
    while (str[*i + len] && (ft_isalnum(str[*i + len]) || str[*i + len] == '_'))
        len++;
    
    if (len == 0)
        return (ft_strdup("$"));

    *i += len;
    var_name = ft_substr(str, start, len);
    if (!var_name)
        return (NULL);

    var_value = get_env_value(shell->env, var_name);
    free(var_name);

    result = var_value ? ft_strdup(var_value) : ft_strdup("");
    return (result);
}

static char *expand_string(char *str, t_shell *shell)
{
    int i;
    char *result;
    char *temp;
    char *var;
    char quote;

    result = ft_strdup("");
    i = 0;
    quote = 0;

    while (str[i])
    {
        if (!quote && (str[i] == '\'' || str[i] == '"'))
            quote = str[i];
        else if (quote && str[i] == quote)
            quote = 0;
        else if (str[i] == '$' && quote != '\'')
        {
            var = expand_single_var(str, &i, shell);
            temp = ft_strjoin(result, var);
            free(result);
            free(var);
            result = temp;
            continue;
        }

        temp = ft_charjoin(result, str[i]);
        free(result);
        result = temp;
        i++;
    }

    return (result);
}

void expand_variables(t_command *cmd, t_shell *shell)
{
    int i;
    char *expanded;

    while (cmd)
    {
        if (cmd->args)
        {
            i = 0;
            while (cmd->args[i])
            {
                expanded = expand_string(cmd->args[i], shell);
                free(cmd->args[i]);
                cmd->args[i] = expanded;
                i++;
            }
        }

        if (cmd->input_file)
        {
            expanded = expand_string(cmd->input_file, shell);
            free(cmd->input_file);
            cmd->input_file = expanded;
        }

        if (cmd->output_file)
        {
            expanded = expand_string(cmd->output_file, shell);
            free(cmd->output_file);
            cmd->output_file = expanded;
        }

        cmd = cmd->next;
    }
}
