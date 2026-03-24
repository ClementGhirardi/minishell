/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:51:27 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/24 14:00:19 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	here_doc(char *limiter)
{
	char	*buffer;
	int		size;
	int		fd[2];
	int		f = open("f", O_WRONLY);

	pipe(fd);
	size = ft_strlen(limiter);
	buffer = readline("> ");
	// write(1, "> ", 2);
	// buffer = get_next_line(STDIN_FILENO);
	if (!buffer)
		perror("minishell");
	while (ft_strncmp(buffer, limiter, size))
	{
		ft_putstr_fd(buffer, fd[1]);
		ft_putstr_fd(buffer, f);
		free(buffer);
		buffer = readline("> ");
		// write(1, "> ", 2);
		// buffer = get_next_line(STDIN_FILENO);
		if (!buffer)
			perror("minishell");
	}
	free(buffer);
	close(fd[1]);
	return (fd[0]);
}
