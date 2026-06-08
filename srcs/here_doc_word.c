/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:26:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 14:30:02 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_sigint_heredoc_word(t_data *data)
{
	if (g_sig_status != 4)
	{
		ft_putendl_fd("minishell: syntax error: unexpected end of file", 1);
		ft_putendl_fd("exit", 1);
		free(*data->input);
		ft_exit(data, NULL, -1, 1);
	}
	return (1);
}

int	here_doc_word(char limiter, t_data *data)
{
	char	*buffer;
	int		nb_line;

	nb_line = -1;
	if (g_sig_status == 4)
		return (1);
	// if (data->update_history == 1)
	// printf("input:|%s|\n", *data->input);
	add_history(*data->input);
	free(*data->input);
	*data->input = ft_strdup("");
	buffer = malloc(1);
	if (!buffer)
		return (1);
	buffer[0] = '\0';
	while (*buffer == '\n' || (!ft_is_in(limiter, buffer) && g_sig_status != 4))
	{
		free(buffer);
		if (!get_buffer(&buffer, &nb_line, data, NULL))
			return (handle_sigint_heredoc_word(data));
		*data->input = ft_strjoin_and_free(*data->input,
				ft_substr(buffer, 0, ft_strlen(buffer) - 1));
	}
	free(buffer);
	return (1);
}
