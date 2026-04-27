/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:51:27 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/27 16:43:21 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_string(char *str, int status, char **env)
{
	int		i;
	char	*result;
	char	*var;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (free(str), NULL);
	while (str[i])
	{
		if (str[i] == '$')
			var = extract_var_name(str, &i, status, env);
		else
			var = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, var);
	}
	return (free(str), result);
}

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
	return (1);
}

int	read_previous(t_data *data, int status, char **env, int *fd)
{
	t_var	v;
	size_t	i;

	if (!begin(data, &v))
		return (0);
	i = 1;
	if (i == ft_strlen(v.value))
		return (0);
	while ((v.value)[i])
	{
		v.len = idx_next_line(&(v.value)[i]);
		if (v.len == 0)
			return (free_token(v.current), (v.previous)->next = NULL, 0);
		v.line = ft_substr(v.value, i, v.len);
		i += v.len;
		if (!ft_strncmp(data->limiter, v.line, ft_strlen(v.line) - 1))
		{
			if (i == ft_strlen(v.value))
				return (free_token(v.current), (v.previous)->next = NULL, 1);
			return (v.new_value = ft_substr(v.value, i, ft_strlen(v.value) - i),
				free(v.value), (v.current)->value = v.new_value, 1);
		}
		ft_putstr_fd(expand_string(v.line, status, env), fd[1]);
	}
	return (free_token(v.current), (v.previous)->next = NULL, 0);
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
	return (free(buffer), close(fd[1]), fd[0]);
}
