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

static int	read_previous(char *limiter, t_data *data, int *fd)
{
	char	*line;
	int		i;
	char	*tmp;

	if (!data->other_lines)
		return (0);
	i = 0;
	line = get_one_line(data->other_lines, &i);
	while (line)
	{
		if (!ft_strncmp(line, limiter, ft_strlen(limiter))
			&& ft_strlen(limiter) == ft_strlen(line) - 1)
		{
			free(line);
			if (i >= (int) ft_strlen(data->other_lines))
				return (free(data->other_lines), 1);
			return (tmp = ft_substr(data->other_lines,
					i, ft_strlen(data->other_lines) - i),
				free(data->other_lines), data->other_lines = tmp, 1);
		}
		ft_putstr_fd(line, fd[1]);
		free(line);
		line = get_one_line(data->other_lines, &i);
	}
	return (free(data->other_lines), data->other_lines = NULL, 0);
}

static int	condition(char *limiter, char *buffer)
{
	return ((ft_strncmp(buffer, limiter, ft_strlen(limiter))
			|| ft_strlen(limiter) != ft_strlen(buffer) - 1
			|| !ft_strncmp(buffer, "\n", 1)) && g_sig_status != 4);
}

void	add_history_noendl(char *str)
{
	char	*tmp;

	if (!str)
		return ;
	if (ft_strlen(str) < 2)
		return ;
	tmp = ft_substr(str, 0 , ft_strlen(str) - 1);
	add_history(tmp);
	free(tmp);
}

static void	write_buffer(t_data *data, char *buffer, int *fd)
{
	char	*tmp;

	tmp = ft_strdup(buffer);
	*data->input = ft_strjoin_and_free(*data->input, tmp);
	add_history(*data->input);
	buffer = expand_only_var(buffer, *data->status, data->env);
	ft_putstr_fd(buffer, fd[1]);
	free(buffer);
}

static int	last_here_doc(t_data *data, t_ast *current, char *buffer, int *fd)
{
	current = current->left;
	while (current)
	{
		if (current->type == NODE_HEREDOC)
			break ;
		current = current->left;
	}
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup("\n"));
	if (g_sig_status == 4)
		return (free(buffer), close(fd[1]), close(fd[0]), -1);
	return (free(buffer), close(fd[1]), fd[0]);
}

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
