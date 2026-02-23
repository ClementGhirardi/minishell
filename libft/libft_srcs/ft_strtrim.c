/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 19:53:06 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/12 19:03:08 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_isin(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	s;
	size_t	e;
	char	*newstr;
	size_t	i;

	if (!s1 || !set)
		return (NULL);
	s = 0;
	while (s1[s] && ft_isin(s1[s], set))
		s++;
	e = ft_strlen(s1);
	while (e > s && ft_isin(s1[e - 1], set))
		e--;
	newstr = (char *)malloc(sizeof(char) * (e - s + 1));
	if (!newstr)
		return (NULL);
	i = 0;
	while (s + i < e)
	{
		newstr[i] = s1[s + i];
		i++;
	}
	newstr[i] = '\0';
	return (newstr);
}
