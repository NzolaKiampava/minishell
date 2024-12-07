/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 21:23:16 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 21:23:16 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strcmp(const char *s1, const char *s2)
{
    // Compara caracteres até encontrar diferença ou terminador nulo
    while (*s1 != '\0' && *s2 != '\0') {
        // Se os caracteres forem diferentes, retorna a diferença
        if (*s1 != *s2)
            return (unsigned char)*s1 - (unsigned char)*s2;
        
        // Avança para os próximos caracteres
        s1++;
        s2++;
    }

    // Verifica se uma das strings terminou antes da outra
    return (unsigned char)*s1 - (unsigned char)*s2;
}
