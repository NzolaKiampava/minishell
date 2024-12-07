/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 21:17:34 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 21:17:34 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strcpy(char *dest, const char *src)
{
    char *original_dest = dest;

    // Copia caracteres até encontrar o terminador nulo
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }

    // Adiciona o terminador nulo no final
    *dest = '\0';

    // Retorna o ponteiro para o início da string destino
    return original_dest;
}