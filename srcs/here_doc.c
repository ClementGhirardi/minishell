/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:51:27 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/26 11:37:16 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	here_doc(char *limiter)
{
	char	*buffer;
	int		size;
	int		fd[2];

	pipe(fd);
	size = ft_strlen(limiter);
	write(1, "> ", 2);
	buffer = get_next_line(STDIN_FILENO);
	if (!buffer)
		perror("minishell");
	while (ft_strncmp(buffer, limiter, size))
	{
		ft_putstr_fd(buffer, fd[1]);
		free(buffer);
		write(1, "> ", 2);
		buffer = get_next_line(STDIN_FILENO);
		if (!buffer)
			perror("minishell");
	}
	free(buffer);
	close(fd[1]);
	return (fd[0]);
}
