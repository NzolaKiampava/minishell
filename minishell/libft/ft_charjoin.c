/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_charjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-07 11:47:17 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-07 11:47:17 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_charjoin(char *result, char c)
{
	char	*new_str;
	int		i;
	int		len;

	if (!result)
	{
		new_str = malloc(sizeof(char) * 2);
		if (!new_str)
			return (NULL);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	len = ft_strlen(result);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (NULL);
	i = -1;
	while (result[++i])
		new_str[i] = result[i];
	new_str[i] = c;
	new_str[i + 1] = '\0';
	return (new_str);
}
