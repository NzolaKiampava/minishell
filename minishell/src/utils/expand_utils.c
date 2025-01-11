/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-30 17:38:29 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-30 17:38:29 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(char *var_value)
{
	if (var_value)
		return (ft_strdup(var_value));
	return (ft_strdup(""));
}

char	*get_var_name(char *str, int *i, int start)
{
	int	len;

	len = 0;
	while (str[*i + len] && (ft_isalnum(str[*i + len]) || str[*i + len] == '_'))
		len++;
	*i += len;
	return (ft_substr(str, start, len));
}

char	*process_quotes(char *str, char *result, int *i, int *j)
{
	int	len;

	len = ft_strlen(str);
	if ((str[*i] == '\'' || str[*i] == '"')
		&& *i == 0 && str[len - 1] == str[*i])
		(*i)++;
	else if ((str[*i] == '\'' || str[*i] == '"')
		&& *i == len - 1 && str[0] == str[*i])
		(*i)++;
	else
		result[(*j)++] = str[(*i)++];
	return (result);
}

char	*strip_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
		result = process_quotes(str, result, &i, &j);
	result[j] = '\0';
	return (result);
}

char	*expand_single_var(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*result;
	int		start;

	(*i)++;
	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	if (!ft_isalnum(str[*i]) && str[*i] != '_')
		return (ft_strdup("$"));
	var_name = get_var_name(str, i, start);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(shell->env, var_name);
	free(var_name);
	result = get_var_value(var_value);
	return (result);
}
