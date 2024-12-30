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

char    *process_quotes(char c, int *quotes)
{
    if (c == '\'' && !quotes[1])
        quotes[0] = !quotes[0];
    else if (c == '"' && !quotes[0])
        quotes[1] = !quotes[1];
    return NULL;
}

char    *process_expansion(char *str, int *i, char *result, t_shell *shell)
{
    char    *temp;
    char    *var;

    if (str[*i] == '$')
    {
        var = expand_single_var(str, i, shell);
        temp = ft_strjoin(result, var);
        free(result);
        free(var);
        return temp;
    }
    temp = ft_charjoin(result, str[*i]);
    free(result);
    return temp;
}