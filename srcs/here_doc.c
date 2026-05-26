/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:51:27 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 15:55:20 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	begin(t_data *data, t_var *v)
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

static int	when_find_limiter(size_t i, t_var *v)
{
	if (i == ft_strlen(v->value))
		return (free_token(v->current), (v->previous)->next = NULL, 1);
	v->new_value = ft_substr(v->value, i, ft_strlen(v->value) - i);
	if (!v->new_value)
		return (1);
	return (free(v->value), (v->current)->value = v->new_value, 1);
}

static int	read_previous(t_data *data, int status, char **env, int *fd)
{
	t_var	v;
	size_t	i;

	if (!begin(data, &v))
		return (add_history(*data->input), 0);
	i = 0;
	while ((v.value)[i])
	{
		v.len = idx_to_next_line(&(v.value)[i]);
		if (v.len == 0)
			return (free_token(v.current), (v.previous)->next = NULL,
				add_history(*data->input), 0);
		v.line = ft_substr(v.value, i, v.len);
		if (!v.line)
			return (add_history(*data->input), 1);
		i += v.len;
		if (!ft_strncmp(data->limiter, v.line, v.len - 1) && v.len != 1)
			return (add_history(*data->input), when_find_limiter(i, &v));
		ft_putstr_fd(expand_string(v.line, status, env), fd[1]);
	}
	return (ft_putstr_fd("\n", fd[1]),
		free_token(v.current), (v.previous)->next = NULL,
		add_history(*data->input), 0);
}







char	*expand_only_var(char *str, int status, char **env)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			tmp = extract_var_name(str, &i, status, env);
		else
			tmp = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, tmp);
	}
	return (free(str), result);
}

static int	last_here_doc(t_data *data)
{
	t_token	*current;

	*data->input = ft_strjoin_and_free(*data->input, ft_strdup(data->limiter));
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

int	here_doc(t_data *data, int *status, char **env)
{
	static int	nb_line;
	char		*buffer;
	int			fd[2];
	char		*tmp;

	if (g_sig_status == 4)
		return (-1);
	if (pipe(fd) < 0)
		return (-1);
	if (read_previous(data, *status, env, fd))
		return (close(fd[1]), fd[0]);
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup("\n"));
	if (!get_buffer(&buffer, &nb_line, status, env))
		return (error_here_doc(fd, nb_line, data->limiter, *status));
	// ft_printf("buffer:%s\n", buffer);
	while ((ft_strncmp(buffer, data->limiter, ft_strlen(buffer) - 1)
			|| !ft_strncmp(buffer, "\n", 1))
		&& g_sig_status != 4)
	{
		tmp = ft_strdup(buffer);
		*data->input = ft_strjoin_and_free(*data->input, tmp);
		add_history(*data->input);
		buffer = expand_only_var(buffer, *status, env);
		// ft_printf("buffer:%s\n", buffer);
		ft_putstr_fd(buffer, fd[1]);
		if (!get_buffer(&buffer, &nb_line, status, env))
			return (error_here_doc(fd, nb_line, data->limiter, *status));
	}
	last_here_doc(data);
	if (g_sig_status == 4)
		return (free(buffer), close(fd[1]), close(fd[0]), -1);
	return (free(buffer), close(fd[1]), fd[0]);
}
