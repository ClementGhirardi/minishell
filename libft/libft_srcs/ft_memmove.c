/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:24:13 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/12 18:59:43 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*strdest;
	const unsigned char	*strsrc;

	if (!dest && !src)
		return (NULL);
	strdest = (unsigned char *) dest;
	strsrc = (const unsigned char *) src;
	i = 0;
	while (i < n)
	{
		if (&(strsrc[i]) < &(strdest[i]))
			strdest[n - 1 - i] = strsrc[n - 1 - i];
		else
			strdest[i] = strsrc[i];
		i++;
	}
	return (strdest);
}
