/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:35:53 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/04 13:33:36 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	size_t	len_s1;
	size_t	len_s2;
	size_t	i;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	newstr = malloc ((len_s1 + len_s2 + 1) * sizeof(char));
	if (!newstr)
		return (NULL);
	i = 0;
	while (i < len_s1)
	{
		newstr[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < len_s2)
	{
		newstr[len_s1 + i] = s2[i];
		i++;
	}
	newstr[len_s1 + i] = '\0';
	return (newstr);
}
