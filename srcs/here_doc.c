/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:51:27 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/24 10:51:38 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	here_doc(char *limiter)
{
	char	*buffer;
	int		size;
	int		fd[2];

	pipe(fd);
	write(1, "> ", 2);
	size = ft_strlen(limiter);
	buffer = get_next_line(STDIN_FILENO);
	while (ft_strncmp(buffer, limiter, size) || buffer[size] != '\n')
	{
		ft_putstr_fd(buffer, fd[1]);
		write(1, "> ", 2);
		free(buffer);
		buffer = get_next_line(STDIN_FILENO);
	}
	free(buffer);
	close(fd[1]);
	return (fd[0]);
}
