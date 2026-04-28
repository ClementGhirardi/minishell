/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:51:27 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/28 11:37:54 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	idx_next_line(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i + 1);
		i++;
	}
	return (i);
}

int	begin(t_data *data, t_var *v)
{
	v->current = *data->tokens;
	if (!v->current)
		return (0);
	v->previous = v->current;
	v->current = (v->current)->next;
	while (v->current && !ft_strchr((v->current)->value, '\n'))
	{
		v->previous = v->current;
		v->current = (v->current)->next;
	}
	if (!v->current)
		return (0);
	v->value = (v->current)->value;
	if (ft_strlen(v->value) == 0)
		return (0);
	return (1);
}

int	read_previous(t_data *data, int status, char **env, int *fd)
{
	t_var	v;
	size_t	i;

	if (!begin(data, &v))
		return (0);
	i = 0;
	while ((v.value)[i])
	{
		v.len = idx_next_line(&(v.value)[i]);
		if (v.len == 0)
			return (free_token(v.current), (v.previous)->next = NULL, 0);
		v.line = ft_substr(v.value, i, v.len);
		i += v.len;
		if (!ft_strncmp(data->limiter, v.line, v.len - 1) && v.len != 1)
		{
			if (i == ft_strlen(v.value))
				return (free_token(v.current), (v.previous)->next = NULL, 1);
			return (v.new_value = ft_substr(v.value, i, ft_strlen(v.value) - i),
				free(v.value), (v.current)->value = v.new_value, 1);
		}
		ft_putstr_fd(expand_string(v.line, status, env), fd[1]);
	}
	return (ft_putstr_fd("\n", fd[1]),
		free_token(v.current), (v.previous)->next = NULL, 0);
}

int	last_here_doc(t_data *data)
{
	t_token	*current;

	current = *data->tokens;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			return (0);
		current = current->next;
	}
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup("\n"));
	return (1);
}

int	here_doc(t_data *data, int status, char **env)
{
	char	*buffer;
	int		size;
	int		fd[2];

	pipe(fd);
	if (read_previous(data, status, env, fd))
		return (close(fd[1]), fd[0]);
	size = ft_strlen(data->limiter);
	write(1, "> ", 2);
	buffer = expand_string(get_next_line(STDIN_FILENO), status, env);
	if (!buffer)
		perror("gnl");
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup("\n"));
	while (ft_strncmp(buffer, data->limiter, size))
	{
		ft_putstr_fd(buffer, fd[1]);
		*data->input = ft_strjoin_and_free(*data->input, buffer);
		write(1, "> ", 2);
		buffer = expand_string(get_next_line(STDIN_FILENO), status, env);
		if (!buffer)
			perror("gnl");
	}
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup(data->limiter));
	last_here_doc(data);
	return (free(buffer), close(fd[1]), fd[0]);
}
