/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gethole_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:33:57 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/15 16:38:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_gethole_fd(int fd)
{
	char	*result;
	char	*buffer;

	result = ft_strdup("");
	buffer = get_next_line(fd);
	while (buffer)
	{
		result = ft_strjoin_and_free(result, buffer);
		buffer = get_next_line(fd);
	}
	return (result);
}
