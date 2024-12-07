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

char *find_command_path(char *cmd, char **env)
{
    char *path;
    char **paths;
    char *full_path;
    char *temp;
    int i;

    if (ft_strchr(cmd, '/'))
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
        temp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(temp, cmd);
        free(temp);

        if (access(full_path, X_OK) == 0)
        {
            i = 0;
            while (paths[i])
                free(paths[i++]);
            free(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }

    i = 0;
    while (paths[i])
        free(paths[i++]);
    free(paths);
    return (NULL);
}

static int execute_external_command(t_command *cmd, t_shell *shell)
{
    char *cmd_path;
    pid_t pid;
    int status;

    if (!cmd->args || !cmd->args[0])
        return (0);

    cmd_path = find_command_path(cmd->args[0], shell->env);
    if (!cmd_path)
    {
        print_error("command not found");
        return (127);
    }

    pid = fork();
    if (pid == 0)
    {
        if (handle_redirections(cmd) != 0)
            exit(1);
        
        execve(cmd_path, cmd->args, shell->env);
        print_error("execution failed");
        exit(1);
    }
    else if (pid < 0)
    {
        print_error("fork failed");
        free(cmd_path);
        return (1);
    }

    free(cmd_path);
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}

int execute_commands(t_shell *shell)
{
    t_command *cmd;
    int exit_status;

    cmd = shell->commands;
    if (!cmd)
        return (0);

    // Se houver mais de um comando, use pipes
    if (cmd->next)
        return (handle_pipes(cmd, shell));

    // Se for um built-in, execute-o
    if (is_builtin(cmd->args[0]))
        return (execute_builtin(cmd, shell));

    // Caso contrário, execute como comando externo
    exit_status = execute_external_command(cmd, shell);
    return (exit_status);
}
