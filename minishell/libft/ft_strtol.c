/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-06 20:51:31 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-06 20:51:31 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long ft_strtol(const char *nptr, char **endptr, int base)
{
    long result = 0;
    int sign = 1;
    int overflow = 0;

    // Pula espaços em branco iniciais
    while (*nptr == ' ' || *nptr == '\t')
        nptr++;

    // Verifica o sinal
    if (*nptr == '-') {
        sign = -1;
        nptr++;
    } else if (*nptr == '+') {
        nptr++;
    }

    // Verifica a base
    if (base == 0) {
        // Detecção automática da base
        if (*nptr == '0') {
            nptr++;
            if (*nptr == 'x' || *nptr == 'X') {
                base = 16;
                nptr++;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    }

    // Conversão
    while (*nptr) {
        int digit;
        
        // Converte o caractere para dígito
        if (*nptr >= '0' && *nptr <= '9')
            digit = *nptr - '0';
        else if (*nptr >= 'a' && *nptr <= 'z')
            digit = *nptr - 'a' + 10;
        else if (*nptr >= 'A' && *nptr <= 'Z')
            digit = *nptr - 'A' + 10;
        else
            break;

        // Verifica se o dígito é válido para a base
        if (digit >= base)
            break;

        // Verifica overflow
        if (!overflow) {
            long prev = result;
            result = result * base + digit;
            
            // Verifica se houve overflow
            if (result / base != prev)
                overflow = 1;
        }

        nptr++;
    }

    // Configura o endptr se fornecido
    if (endptr)
        *endptr = (char *)nptr;

    // Aplica o sinal
    result *= sign;

    // Lida com overflow
    if (overflow) {
        // Em caso de overflow, retorna LONG_MAX ou LONG_MIN
        return (sign == 1) ? LONG_MAX : LONG_MIN;
    }

    return result;
}
