/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:41:31 by nkiampav          #+#    #+#             */
/*   Updated: 2024/12/06 12:41:33 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função para encontrar o caminho completo do executável
char *get_command_path(char *cmd, char **env)
{
    char *path;
    char **paths;
    char *full_path;
    int i;

    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd));
    path = get_env_value(env, "PATH");
    if (!path)
        return (NULL);
    paths = ft_split(path, ':');
    if (!paths)
        return (NULL);
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

static int execute_single_command(t_command *cmd, t_shell *shell)
{
    pid_t pid;
    int status;
    char *path;
    
    // Salvar os file descriptors originais
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);

    // Se for um builtin, aplicar redirecionamentos antes de executar
    if (is_builtin(cmd->args[0]))
    {
        if (!handle_redirections(cmd))
        {
            // Restaurar os file descriptors originais em caso de erro
            dup2(saved_stdout, STDOUT_FILENO);
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdout);
            close(saved_stdin);
            return (1);
        }
        
        // Executar o builtin
        status = execute_builtin(cmd, shell);
        
        // Restaurar os file descriptors originais
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
        
        return status;
    }

    // Para comandos não-builtin, continuar com o processo normal
    pid = fork();
    if (pid == -1)
        return (1);
    if (pid == 0)
    {
        if (!handle_redirections(cmd))
            exit(1);
        path = get_command_path(cmd->args[0], shell->env);
        if (!path)
        {
            print_error("Command not found");
            exit(127);
        }
        execve(path, cmd->args, shell->env);
        free(path);
        exit(1);
    }
    waitpid(pid, &status, 0);
    return (WEXITSTATUS(status));
}

int	execute_commands(t_shell *shell)
{
    t_command	*cmd;
    int			has_pipe;

    cmd = shell->commands;
    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);

    // Check if we have any pipes
    has_pipe = 0;
    for (t_command *tmp = cmd; tmp->next; tmp = tmp->next)
        has_pipe = 1;

    if (has_pipe)
        return (execute_piped_commands(cmd, shell));
    else
        return (execute_single_command(cmd, shell));
}
