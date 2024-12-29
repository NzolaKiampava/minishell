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

char	*ft_strjoin_char(char *result, char c)
{
	int		len;
	char	*new_str;

	if (result == NULL)
	{
		new_str = malloc(2 * sizeof(char));
		if (new_str == NULL)
			return (NULL);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	len = ft_strlen(result);
	new_str = malloc((len + 2) * sizeof(char));
	if (new_str == NULL)
		return (NULL);
	ft_strcpy(new_str, result);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(result);
	return (new_str);
}
