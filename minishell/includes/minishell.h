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
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

/*
** Token types for parsing
*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

/*
** Token structure
*/
typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}	t_token;

/*
** Command structure
*/
typedef struct s_command
{
	char				**args;
	char				*input_file;
	char				*output_file;
	int					input_fd;
	int					output_fd;
	int					append_mode;
	struct s_command	*next;
}	t_command;

/*
** Main shell structure
*/
typedef struct s_shell
{
	char				**env;
	int					exit_status;
	int					running;
	t_command			*commands;
	t_token				*tokens;
}	t_shell;

/*
** Global variable for signal handling
*/
extern int	g_signal_received;

/*
** Function prototypes
*/

/* Parser functions */
t_token		*tokenize_input(char *input);
t_command	*parse_tokens(t_token *tokens);
void		expand_variables(t_command *cmd, t_shell *shell);

/* Executor functions */
int			execute_commands(t_shell *shell);
int			execute_piped_commands(t_command *cmd, t_shell *shell);
int			handle_redirections(t_command *cmd);
char		*get_command_path(char *cmd, char **env);

/* Builtin functions */
int			is_builtin(char *cmd);
int			execute_builtin(t_command *cmd, t_shell *shell);
int			ft_echo(char **args);
int			ft_cd(char **args, t_shell *shell);
int			ft_pwd(void);
int			ft_export(char **args, t_shell *shell);
int			ft_unset(char **args, t_shell *shell);
int			ft_env(t_shell *shell);
int			ft_exit(char **args, t_shell *shell);
char		*expand_env_vars(char *str, t_shell *shell);

/* Signal handlers */
void		setup_signals(void);
void		handle_signal(int signo);

/* Environment functions */
char		**init_env(char **env);
char		*get_env_value(char **env, char *key);
int			set_env_value(char ***env, char *key, char *value);

/* Utils functions */

void		free_tokens(t_token *tokens);
void		free_commands(t_command *commands);
void		free_shell(t_shell *shell);
void		cleanup_pipes(int prev_pipe[2], int curr_pipe[2], int has_next);
void		print_error(char *msg);

/* Parser Utils Functions */
int			validate_redirect_syntax(t_token *token);
int			validate_pipe_syntax(t_token *tokens);
char		*search_in_path(char **paths, char *cmd);

/* Tokenizer Utils Functions */
void		add_token(t_token **head, char *value, t_token_type type);
char		*get_content_between_quotes(char *input, int *i, char quote_type,
				int preserve_quotes);
char		*get_unquoted_content(char *input, int *i);
int			should_preserve_quotes(t_token *head);
char		*join_word_parts(char *result, char *next_part);
char		*process_word_part(char *input, int *i, char *result,
				t_token *head);
void		process_operator(char *input, int *i, char **value,
				t_token_type *type);

/* Expander Utils Functions */
char		*process_quotes(char c, int *quotes);
char		*process_expansion(char *str, int *i, char *result, t_shell *shell);
char		*get_var_name(char *str, int *i, int *len);
int			is_quoted(char *str, int len);

/* Pipe Utils Functins */
int			setup_pipes(int curr_pipe[2], int prev_pipe[2], t_command *current);
void		redirect_pipes(int prev_pipe[2], int curr_pipe[2]);

/* UTILS */
int			is_space(char c);
int			is_operator(char c);
int			add_arg(t_command *cmd, char *arg);
t_command	*create_command(void);

#endif
