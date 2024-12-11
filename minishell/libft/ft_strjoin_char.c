/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 19:40:06 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 19:40:06 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strjoin_char(char *result, char c)
{
    int len;
    char *new_str;

    // Se result for NULL, cria uma nova string
    if (result == NULL)
    {
        new_str = malloc(2 * sizeof(char));
        if (new_str == NULL)
            return NULL;
        new_str[0] = c;
        new_str[1] = '\0';
        return new_str;
    }

    // Calcula o comprimento da string existente
    len = ft_strlen(result);

    // Aloca memória para a nova string (len + 2 para o novo char e terminador nulo)
    new_str = malloc((len + 2) * sizeof(char));
    if (new_str == NULL)
        return NULL;

    // Copia a string original
    ft_strcpy(new_str, result);

    // Adiciona o novo caractere no final
    new_str[len] = c;
    new_str[len + 1] = '\0';

    // Libera a memória da string original
    free(result);

    return new_str;
}