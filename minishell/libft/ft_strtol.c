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

static void	skip_whitespace_and_sign(const char **nptr, int *sign)
{
	while (**nptr == ' ' || **nptr == '\t')
		(*nptr)++;
	*sign = 1;
	if (**nptr == '-')
	{
		*sign = -1;
		(*nptr)++;
	}
	else if (**nptr == '+')
		(*nptr)++;
}

static int	detect_base(const char **nptr, int base)
{
	if (base == 0)
	{
		if (**nptr == '0')
		{
			(*nptr)++;
			if (**nptr == 'x' || **nptr == 'X')
			{
				(*nptr)++;
				return (16);
			}
			return (8);
		}
		return (10);
	}
	return (base);
}

static int	get_digit_value(char c, int base)
{
	int	digit;

	if (c >= '0' && c <= '9')
		digit = c - '0';
	else if (c >= 'a' && c <= 'z')
		digit = c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z')
		digit = c - 'A' + 10;
	else
		return (-1);
	if (digit >= base)
		return (-1);
	return (digit);
}

static long	process_digits(const char **nptr, int base, int *overflow)
{
	long			result;
	long			prev;
	int				digit;

	result = 0;
	digit = get_digit_value(**nptr, base);
	while (digit >= 0)
	{
		if (!*overflow)
		{
			prev = result;
			result = result * base + digit;
			if (result / base != prev)
				*overflow = 1;
		}
		(*nptr)++;
		digit = get_digit_value(**nptr, base);
	}
	return (result);
}

long	ft_strtol(const char *nptr, char **endptr, int base)
{
	long			result;
	int				sign;
	int				overflow;

	overflow = 0;
	skip_whitespace_and_sign(&nptr, &sign);
	base = detect_base(&nptr, base);
	result = process_digits(&nptr, base, &overflow);
	if (endptr)
		*endptr = (char *)nptr;
	result *= sign;
	if (overflow)
	{
		if (sign == 1)
			return (LONG_MAX);
		else
			return (LONG_MIN);
	}
	return (result);
}
