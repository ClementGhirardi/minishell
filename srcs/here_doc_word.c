/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:26:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 17:54:54 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_isin(char c, char *str)
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

char	*here_doc_word(char limiter, t_data *data)
{
	char	*buffer;
	int		nb_line;

	nb_line = -1;
	if (g_sig_status == 4)
		return (NULL);
	add_history(*data->input);
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup(" "));
	if (!get_buffer(&buffer, &nb_line, data))
		return (free(*data->input), NULL);
	while (!ft_isin(limiter, buffer) && g_sig_status != 4)
	{
		*data->input = ft_strjoin_and_free(*data->input, buffer);
		if (!get_buffer(&buffer, &nb_line, data))
			return (*data->input);
	}
	*data->input = ft_strjoin_and_free(*data->input,
			ft_substr(buffer, 0, ft_strlen(buffer) - 1));
	if (g_sig_status == 4)
		return (free(buffer), *data->input);
	return (free(buffer), *data->input);
}
