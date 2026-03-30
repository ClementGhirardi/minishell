/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 00:25:08 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/31 00:48:56 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**dup_array(char **array)
{
	char	**new_array;
	int		size;
	int		i;

	if (!array)
		return (NULL);
	size = 0;
	while (array[size])
		size++;
	new_array = malloc((size + 1) * sizeof(char *));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
			return (free_array(new_array), NULL);
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}
