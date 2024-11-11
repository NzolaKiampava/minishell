/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:58:16 by nkiampav          #+#    #+#             */
/*   Updated: 2024/11/11 14:09:19 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <signal.h>
# include "libft.h"

# define PROMPT "minishell$ "

typedef struct	s_command
{
	char	*cmd;
	char	**args;
	char	*input_file;
	char	*output_file;
	int	append_mode;
	char	*heredoc_delim;
	struct s_command	*next;
}	t_command;

// Main functions
void	init_shell(void);
char	*read_input(void);
void	add_to_history(char *input);
int	parse_input(char *input, t_command **cmd);
int	execute_command(t_command *cmd);

// Signal handling
void	setup_signals(void);
void	handle_signal(int signo);

// Built-in commands
int	es_buitin(char *cmd);
int	execute_builtin(t_command *cmd);
int	ft_echo(char **args);
int	ft_cd(char **args);
int	ft_pwd(void);
int	ft_export(char	**args);
int	ft_unset(char	**args);
int	ft_env(void);
int	ft_exit(char **args);

// Memory managemment
void	free_command(t_command *cmd);

#endif
