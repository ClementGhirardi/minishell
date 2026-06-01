/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_buffer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:12:40 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/01 14:54:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	ft_isin(char *str, char c)
// {
// 	int	i;

// 	if (!str)
// 		return (0);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == c)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

// static int	child(char **buffer, int *fd)
// {
// 	g_sig_status = 3;
// 	close(fd[0]);
// 	*buffer = readline("> ");
// 	if (*buffer)
// 		ft_putendl_fd(*buffer, fd[1]);
// 	exit(0);
// }

// int	get_buffer(char **buffer, int *nb_line, int *status, char **env)
// {
// 	int		fd[2];
// 	pid_t	pid;
// 	int		tmp;

// 	if (pipe(fd) < 0)
// 		return (0);
// 	pid = fork();
// 	if (pid == 0)
// 		child(buffer, fd);
// 	close(fd[1]);
// 	waitpid(pid, &tmp, 0);
// 	*status = get_status(tmp);
// 	if (*status == 130)
// 		return (g_sig_status = 4, close(fd[0]), 0);
// 	*buffer = get_next_line(fd[0]);
// 	if (!(*buffer))
// 		return (close(fd[0]), 0);
// 	if (!ft_isin(*buffer, '\n'))
// 		return (close(fd[0]), free(*buffer), 0);
// 	if (*nb_line != -1)
// 		*buffer = expand_string(*buffer, *status, env);
// 	(*nb_line)++;
// 	return (close(fd[0]), 1);
// }


static int	ft_isin(char *str, char c)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	child(char **buffer, int *fd, t_data *data, int *to_close_fds)
{
	g_sig_status = 3;
	if (to_close_fds && to_close_fds[0] != -1)
		close(to_close_fds[0]);
	if (to_close_fds && to_close_fds[1] != -1)
		close(to_close_fds[1]);
	close(fd[0]);
	free_data(data);
	*buffer = readline("> ");
	if (g_sig_status == 4)
	{
		*data->status = 130;
		free(*buffer);
		close(fd[1]);
		exit(0);
	}
	if (*buffer)
		ft_putendl_fd(*buffer, fd[1]);
	free(*buffer);
	close(fd[1]);
	exit(0);
}

int	get_buffer(char **buffer, int *nb_line, t_data *data, int *to_close_fds)
{
	int		fd[2];
	pid_t	pid;
	int		tmp;

	if (pipe(fd) < 0)
		return (0);
	pid = fork();
	if (pid == 0)
		child(buffer, fd, data, to_close_fds);
	close(fd[1]);
	waitpid(pid, &tmp, 0);
	*data->status = get_status(tmp);
	if (*data->status == 130)
		return (g_sig_status = 4, close(fd[0]), 0);
	*buffer = get_next_line(fd[0]);
	while (get_next_line(fd[0]))
		;
	if (!(*buffer))
		return (close(fd[0]), 0);
	if (!ft_isin(*buffer, '\n'))
		return (close(fd[0]), free(*buffer), 0);
	(*nb_line)++;
	return (close(fd[0]), 1);
}
