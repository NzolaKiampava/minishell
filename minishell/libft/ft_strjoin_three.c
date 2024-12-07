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

char	*ft_strjoin_three(const char *key, const char *separator, const char *value)
{
	size_t	key_len;
	size_t	sep_len;
	size_t	value_len;
	char	*result;

	if (!key || !separator || !value)
		return (NULL);

	key_len = 0;
	while (key[key_len])
		key_len++;

	sep_len = 0;
	while (separator[sep_len])
		sep_len++;

	value_len = 0;
	while (value[value_len])
		value_len++;

	// Allocate memory for the concatenated string
	result = (char *)malloc(key_len + sep_len + value_len + 1);
	if (!result)
		return (NULL);

	// Copy key
	size_t i = 0;
	while (key[i])
	{
		result[i] = key[i];
		i++;
	}

	// Copy separator
	size_t j = 0;
	while (separator[j])
	{
		result[i + j] = separator[j];
		j++;
	}

	// Copy value
	size_t k = 0;
	while (value[k])
	{
		result[i + j + k] = value[k];
		k++;
	}

	// Null-terminate the string
	result[i + j + k] = '\0';

	return (result);
}