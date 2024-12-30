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

static char	*expand_single_var(char *str, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*var_name;
	char	*var_value;
	char	*result;

	start = *i;
	len = 0;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
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

static char	*strip_quotes(char *str)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"')
			&& i == 0 && str[len - 1] == str[i])
		{
			i++;
			continue ;
		}
		else if ((str[i] == '\'' || str[i] == '"')
			&& i == len - 1 && str[0] == str[i])
		{
			i++;
			continue ;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static char *expand_string(char *str, t_shell *shell)
{
    int i;
    char *result;
    int quotes[2];
    char *final_result;

    result = ft_strdup("");
    i = 0;
    quotes[0] = 0;
    quotes[1] = 0;
    while (str[i])
    {
        process_quotes(str[i], quotes);
        if (str[i] == '$' && !quotes[0])
            result = process_expansion(str, &i, result, shell);
        else
        {
            char *temp = ft_charjoin(result, str[i]);
            free(result);
            result = temp;
        }
        i++;
    }
    final_result = strip_quotes(result);
    free(result);
    return final_result;
}

void	expand_variables(t_command *cmd, t_shell *shell)
{
	int	i;
	char	*expanded;

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
