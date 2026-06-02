/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_buffer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:12:40 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 15:47:46 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	child(char **buffer, int *fd, t_data *data, int *fds)
{
	g_sig_status = 3;
	if (fds && fds[0] != -1)
		close(fds[0]);
	if (fds && fds[1] != -1)
		close(fds[1]);
	close(fd[0]);
	free_data(data);
	*buffer = readline("> ");
	if (g_sig_status == 4)
	{
		free(*buffer);
		close(fd[1]);
		exit(130);
	}
	if (*buffer)
		ft_putendl_fd(*buffer, fd[1]);
	free(*buffer);
	close(fd[1]);
	exit(0);
}

int	get_buffer(char **buffer, int *nb_line, t_data *data, int *fds)
{
	int		fd[2];
	pid_t	pid;
	int		tmp;

	if (pipe(fd) < 0)
		return (0);
	g_sig_status = 5;
	pid = fork();
	if (pid == 0)
		child(buffer, fd, data, fds);
	close(fd[1]);
	waitpid(pid, &tmp, 0);
	g_sig_status = 1;
	*data->status = get_status(tmp);
	if (*data->status == 130)
		return (g_sig_status = 4, close(fd[0]), 0);
	*buffer = get_next_line(fd[0]);
	while (get_next_line(fd[0]))
		;
	if (!(*buffer))
		return (close(fd[0]), 0);
	if (!ft_isin('\n', *buffer))
		return (close(fd[0]), free(*buffer), 0);
	(*nb_line)++;
	return (close(fd[0]), 1);
}
