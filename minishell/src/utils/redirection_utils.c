/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:41:54 by nkiampav          #+#    #+#             */
/*   Updated: 2025/01/11 11:42:02 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_variable_expansion(char *line, int *i, t_shell *shell,
	char *result)
{
	char	*var_value;
	char	*temp;

	var_value = expand_single_var(line, i, shell);
	if (var_value)
	{
		temp = result;
		result = ft_strjoin(result, var_value);
		free(temp);
		free(var_value);
	}
	return (result);
}

static char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char		*result;
	int			i;

	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			result = handle_variable_expansion(line, &i, shell, result);
		else
		{
			result = append_character(result, line[i]);
			i++;
		}
	}
	free(line);
	return (result);
}

int	write_heredoc_content(int *pipe_fd, char *delimiter, t_shell *shell)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_signal_received
			|| (line && !ft_strcmp(line, delimiter)))
		{
			free(line);
			break ;
		}
		expanded_line = expand_heredoc_line(line, shell);
		if (pipe_fd[1] != -1)
		{
			write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
			write(pipe_fd[1], "\n", 1);
		}
		free(expanded_line);
	}
	return (g_signal_received);
}

int	cleanup_on_error(int saved_stdout, int saved_stdin,
	int fd, char *message)
{
	if (fd != -1)
		close(fd);
	if (saved_stdout != -1)
		close(saved_stdout);
	if (saved_stdin != -1)
		close(saved_stdin);
	if (message)
		print_error(message);
	return (0);
}

char	*get_env_value(char **env, char *key)
{
	int	i;
	int	key_len;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0
			&& env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}
