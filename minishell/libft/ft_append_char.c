/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-01 19:28:39 by nkiampav          #+#    #+#             */
/*   Updated: 2025-01-01 19:28:39 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_append_char(char *str, char c)
{
	char	*result;
	int		i;
	int		len;

	len = ft_strlen(str);
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	free(str);
	return (result);
}
