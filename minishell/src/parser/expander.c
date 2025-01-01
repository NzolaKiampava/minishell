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

static char	*handle_variable(char *str, int *i, t_shell *shell, char *result)
{
	char	*var;
	char	*temp;

	var = expand_single_var(str, i, shell);
	temp = ft_strjoin(result, var);
	free(result);
	free(var);
	return (temp);
}

static char	*expand_string(char *str, t_shell *shell)
{
	int		i;
	char	*result;
	int		quotes[2];
	char	*final_result;

	result = ft_strdup("");
	i = 0;
	quotes[0] = 0;
	quotes[1] = 0;
	while (str[i])
	{
		update_quotes(str[i], quotes);
		if (str[i] == '$' && !quotes[0])
			result = handle_variable(str, &i, shell, result);
		else
			result = ft_append_char(result, str[i++]);
	}
	final_result = strip_quotes(result);
	free(result);
	return (final_result);
}

static void	expand_one_file(char **file, t_shell *shell)
{
	char	*expanded;

	if (!*file)
		return ;
	expanded = expand_string(*file, shell);
	free(*file);
	*file = expanded;
}

static void	expand_command_args(char **args, t_shell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	while (args[i])
	{
		expanded = expand_string(args[i], shell);
		free(args[i]);
		args[i] = expanded;
		i++;
	}
}

void	expand_variables(t_command *cmd, t_shell *shell)
{
	while (cmd)
	{
		if (cmd->args)
			expand_command_args(cmd->args, shell);
		expand_one_file(&cmd->input_file, shell);
		expand_one_file(&cmd->output_file, shell);
		cmd = cmd->next;
	}
}
