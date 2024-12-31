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

int	is_quoted(char *str, int len)
{
	return ((str[0] == '\'' || str[0] == '"') && str[0] == str[len - 1]);
}

char	*get_var_name(char *str, int *i, int *len)
{
	*len = 0;
	while (str[*i + *len] && (ft_isalnum(str[*i + *len])
			|| str[*i + *len] == '_'))
		(*len)++;
	if (*len == 0)
		return (NULL);
	else
		return (ft_substr(str, *i, *len));
}

char	*process_quotes(char c, int *quotes)
{
	if (c == '\'' && !quotes[1])
		quotes[0] = !quotes[0];
	else if (c == '"' && !quotes[0])
		quotes[1] = !quotes[1];
	return (NULL);
}

static char	*expand_single_var(char *str, int *i, t_shell *shell)
{
	char		*var_name;
	char		*var_value;
	char		*result;
	int			len;

	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	var_name = get_var_name(str, i, &len);
	if (!var_name)
		return (ft_strdup("$"));
	*i += len;
	var_value = get_env_value(shell->env, var_name);
	free(var_name);
	if (var_value)
		result = ft_strdup(var_value);
	else
		result = ft_strdup("");
	return (result);
}

char	*process_expansion(char *str, int *i, char *result, t_shell *shell)
{
	char	*temp;
	char	*var;

	if (str[*i] == '$')
	{
		var = expand_single_var(str, i, shell);
		temp = ft_strjoin(result, var);
		free(result);
		free(var);
		return (temp);
	}
	temp = ft_charjoin(result, str[*i]);
	free(result);
	return (temp);
}
