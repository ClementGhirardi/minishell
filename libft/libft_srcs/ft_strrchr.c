/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:06:34 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/12 15:15:50 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	i = len;
	c = c % 256;
	while (i > 0)
	{
		if (s[i] == c)
			return ((char *)&(s[i]));
		i--;
	}
	if (s[i] == c)
		return ((char *)&(s[i]));
	return (NULL);
}
