/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-30 13:46:47 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-30 13:46:47 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_redirect_syntax(t_token *token)
{
	if (!token->next)
		return (0);
	if (token->next->type != TOKEN_WORD)
		return (0);
	if (token->next->next
		&& (token->next->next->type == TOKEN_REDIRECT_IN
			|| token->next->next->type == TOKEN_REDIRECT_OUT
			|| token->next->next->type == TOKEN_REDIRECT_APPEND
			|| token->next->next->type == TOKEN_HEREDOC))
		return (0);
	return (1);
}

int	validate_pipe_syntax(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE)
		return (0);
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE
			&& (!tokens->next || tokens->next->type == TOKEN_PIPE))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}

char	*search_in_path(char **paths, char *cmd)
{
	char		*full_path;
	int			i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd);
		if (!access(full_path, X_OK))
		{
			ft_free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}

void	update_quotes(char c, int quotes[2])
{
	if (c == '\'' && !quotes[1])
		quotes[0] = !quotes[0];
	else if (c == '"' && !quotes[0])
		quotes[1] = !quotes[1];
}

char	*get_command_path(char *cmd, char **env)
{
	char	*path;
	char	**paths;

	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	return (search_in_path(paths, cmd));
}
