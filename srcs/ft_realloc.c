/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:24:24 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/03 13:49:43 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_realloc(void **ptr, size_t size)
{
	unsigned char	**new;
	unsigned char	**p;
	size_t			i;
	size_t			j;

	if (!size)
		return (free_array((char **)ptr), NULL);
	p = (unsigned char **)ptr;
	i = 0;
	new = ft_calloc(1, size);
	if (!new)
		return (NULL);
	while (p && p[i])
	{
		new[i] = ft_calloc(1,
				sizeof(char) * (ft_strlen((const char *)p[i]) + 1));
		if (!new[i])
			return (NULL);
		j = -1;
		while (p[i][++j])
			new[i][j] = p[i][j];
		i++;
	}
	free_array((char **)ptr);
	return ((char **)new);
}
