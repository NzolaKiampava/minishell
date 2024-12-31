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

static char	*strip_quotes(char *str)
{
	int			len;
	int			i;
	int			j;
	int			limit;
	char		*result;

	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	if (is_quoted(str, len))
		i = 1;
	else
		i = 0;
	j = 0;
	limit = len;
	if (is_quoted(str, len))
		limit = len - 1;
	while (str[i] && i < limit)
		result[j++] = str[i++];
	result[j] = '\0';
	return (result);
}

static char	*expand_string(char *str, t_shell *shell)
{
	int			i;
	int			quotes[2];
	char		*result;
	char		*final_result;
	char		*temp;

	result = ft_strdup("");
	i = -1;
	quotes[0] = 0;
	quotes[1] = 0;
	while (str[++i])
	{
		process_quotes(str[i], quotes);
		if (str[i] == '$' && !quotes[0])
			result = process_expansion(str, &i, result, shell);
		else
		{
			temp = ft_charjoin(result, str[i]);
			free(result);
			result = temp;
		}
	}
	final_result = strip_quotes(result);
	free(result);
	return (final_result);
}

static char	*expand_and_replace(char *original, t_shell *shell)
{
	char	*expanded;

	if (!original)
		return (NULL);
	expanded = expand_string(original, shell);
	free(original);
	return (expanded);
}

void	expand_variables(t_command *cmd, t_shell *shell)
{
	int	i;

	while (cmd)
	{
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				cmd->args[i] = expand_and_replace(cmd->args[i], shell);
				i++;
			}
		}
		cmd->input_file = expand_and_replace(cmd->input_file, shell);
		cmd->output_file = expand_and_replace(cmd->output_file, shell);
		cmd = cmd->next;
	}
}
