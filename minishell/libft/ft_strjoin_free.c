/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <nkiampav@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-07 11:25:37 by nkiampav          #+#    #+#             */
/*   Updated: 2024-12-07 11:25:37 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char *new_var, char *value)
{
	char	*temp;
	int		i;
	int		j;
	int		len1;
	int		len2;

	if (!new_var)
	{
		temp = ft_strdup(value);
		return (temp);
	}
	len1 = ft_strlen(new_var);
	len2 = ft_strlen(value);
	temp = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!temp)
		return (NULL);
	i = -1;
	while (new_var[++i])
		temp[i] = new_var[i];
	j = -1;
	while (value[++j])
		temp[i + j] = value[j];
	temp[i + j] = '\0';
	free(new_var);
	return (temp);
}
