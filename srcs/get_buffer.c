/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_buffer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:12:40 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/30 16:34:15 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	child(char **buffer, int *fd)
{
	g_sig_status = 3;
	close(fd[0]);
	ft_putstr_fd("> ", 1);
	*buffer = get_next_line(STDIN_FILENO);
	if (*buffer)
		ft_putstr_fd(*buffer, fd[1]);
	exit(0);
}

int	get_buffer(char **buffer, int *nb_line, int *status, char **env)
{
	int		fd[2];
	pid_t	pid;
	int		tmp;

	if (pipe(fd) < 0)
		return (0);
	pid = fork();
	if (pid == 0)
		child(buffer, fd);
	close(fd[1]);
	waitpid(pid, &tmp, 0);
	*status = get_status(tmp);
	if (*status == 130)
		return (close(fd[0]), 0);
	*buffer = get_next_line(fd[0]);
	if (!(*buffer))
		return (close(fd[0]), 0);
	if (!ft_strncmp(*buffer, "\n", 1))
		return (free(*buffer), 0);
	*buffer = expand_string(*buffer, *status, env);
	(*nb_line)++;
	return (close(fd[0]), 1);
}
