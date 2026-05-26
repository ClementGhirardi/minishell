/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_buffer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:12:40 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 11:21:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	child(char **buffer, int *fd)
{
	g_sig_status = 3;
	close(fd[0]);
	*buffer = readline("> ");
	if (*buffer)
		ft_putendl_fd(*buffer, fd[1]);
	exit(0);
}

int	get_buffer(char **buffer, int *nb_line, int *status, char **env)
{
	int		fd[2];
	pid_t	pid;
	int		tmp;

	(void) env;
	if (pipe(fd) < 0)
		return (0);
	pid = fork();
	if (pid == 0)
		child(buffer, fd);
	close(fd[1]);
	waitpid(pid, &tmp, 0);
	*status = get_status(tmp);
	if (*status == 130)
		return (g_sig_status = 4, close(fd[0]), 0);
	*buffer = get_next_line(fd[0]);
	if (!(*buffer))
		return (close(fd[0]), 0);
	while (get_next_line(fd[0]))
		;
	if (!ft_isin(*buffer, '\n'))
		return (close(fd[0]), free(*buffer), 0);
	// if (*nb_line != -1)
	// 	*buffer = expand_string(*buffer, *status, env);
	(*nb_line)++;
	return (close(fd[0]), 1);
}
