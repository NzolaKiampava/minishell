/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_three.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-05 22:11:54 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-05 22:11:54 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_params(const char *key, const char *separator,
		const char *value, size_t *total_len)
{
	size_t	key_len;
	size_t	sep_len;
	size_t	value_len;

	if (!key || !separator || !value)
		return (0);
	key_len = ft_strlen(key);
	sep_len = ft_strlen(separator);
	value_len = ft_strlen(value);
	*total_len = key_len + sep_len + value_len + 1;
	return (1);
}

static void	copy_key(char *dest, const char *key, size_t *index)
{
	size_t	i;

	i = 0;
	while (key[i])
	{
		dest[i] = key[i];
		i++;
	}
	*index = i;
}

static void	copy_separator(char *dest, const char *separator,
		size_t start, size_t *len)
{
	size_t	i;

	i = 0;
	while (separator[i])
	{
		dest[start + i] = separator[i];
		i++;
	}
	*len = i;
}

char	*ft_strjoin_three(const char *key, const char *separator,
		const char *value)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	total_len;

	if (!check_params(key, separator, value, &total_len))
		return (NULL);
	result = (char *)malloc(total_len);
	if (!result)
		return (NULL);
	copy_key(result, key, &i);
	copy_separator(result, separator, i, &j);
	k = 0;
	while (value[k])
	{
		result[i + j + k] = value[k];
		k++;
	}
	result[i + j + k] = '\0';
	return (result);
}
