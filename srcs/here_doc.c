/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:51:27 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/01 12:17:00 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	here_doc(char *limiter, t_data *data, t_ast *current)
{
	static int	nb_line;
	char		*buffer;
	int			fd[2];

	if (g_sig_status == 4)
		return (-1);
	if (pipe(fd) < 0)
		return (-1);
	if (read_previous(limiter, data, fd))
		return (close(fd[1]), fd[0]);
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup("\n"));
	add_history_noendl(*data->input);
	if (!get_buffer(&buffer, &nb_line, data, fd))
		return (error_here_doc(fd, nb_line, limiter, *data->status));
	while (condition(limiter, buffer) && ((*limiter) || *buffer != '\n'))
	{
		write_buffer(data, buffer, fd);
		if (!get_buffer(&buffer, &nb_line, data, fd))
			return (error_here_doc(fd, nb_line, limiter, *data->status));
	}
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup(limiter));
	return (last_here_doc(data, current, buffer, fd));
}
