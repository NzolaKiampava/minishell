/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-05 21:29:21 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-05 21:29:21 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Count the number of environment variables
static int count_env_vars(char **env) {
    int count = 0;
    if (!env) return 0;
    
    while (env[count]) {
        count++;
    }
    return count;
}

// Initialize environment variables
char **init_env(char **env) {
    int count;
    char **new_env;
    int i;

    // Count existing environment variables
    count = count_env_vars(env);

    // Allocate memory for new environment array (add 1 for NULL terminator)
    new_env = malloc((count + 1) * sizeof(char *));
    if (!new_env) {
        print_error("Memory allocation failed for environment");
        return NULL;
    }

    // Copy environment variables
    for (i = 0; i < count; i++) {
        new_env[i] = ft_strdup(env[i]);
        if (!new_env[i]) {
            // Free previously allocated memory if strdup fails
            while (i > 0) {
                free(new_env[--i]);
            }
            free(new_env);
            print_error("Memory allocation failed for environment variable");
            return NULL;
        }
    }

    // Null-terminate the array
    new_env[count] = NULL;

    return new_env;
}

// Get value of an environment variable

char *get_env_value(char **env, char *key) {
    int i;
    int key_len;

    if (!env || !key) return NULL;

    key_len = ft_strlen(key);

    for (i = 0; env[i]; i++) {
        // Check if the environment variable starts with the key
        // and is followed by '='
        if (ft_strncmp(env[i], key, key_len) == 0 && 
            env[i][key_len] == '=') {
            // Return the value after the '='
            return ft_strdup(env[i] + key_len + 1);
        }
    }

    return NULL;
}


// Set or update an environment variable
int set_env_value(char ***env, char *key, char *value) {
    int i, count;
    char *new_var;
    char **new_env;

    // Validate inputs
    if (!env || !key || !value) return -1;

    // Create new environment variable string
    new_var = ft_strjoin_three(key, "=", value);
    if (!new_var) {
        print_error("Memory allocation failed");
        return -1;
    }

    // Count existing environment variables
    count = count_env_vars(*env);

    // Check if key already exists
    for (i = 0; (*env)[i]; i++) {
        if (ft_strncmp((*env)[i], key, ft_strlen(key)) == 0 && 
            (*env)[i][ft_strlen(key)] == '=') {
            // Replace existing variable
            free((*env)[i]);
            (*env)[i] = new_var;
            return 0;
        }
    }

    // If key doesn't exist, create new environment array
    new_env = malloc((count + 2) * sizeof(char *));
    if (!new_env) {
        free(new_var);
        print_error("Memory allocation failed");
        return -1;
    }

    // Copy existing environment
    for (i = 0; i < count; i++) {
        new_env[i] = (*env)[i];
    }

    // Add new variable
    new_env[count] = new_var;
    new_env[count + 1] = NULL;

    // Free old environment array and update pointer
    free(*env);
    *env = new_env;

    return 0;
}
