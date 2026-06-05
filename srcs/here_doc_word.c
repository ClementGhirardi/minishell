/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:26:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/05 15:54:58 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	here_doc_word(char limiter, t_data *data)
{
	char	*buffer;
	int		nb_line;

	nb_line = -1;
	if (g_sig_status == 4)
		return ;
	if (data->update_history == 1)
		add_history(*data->input);
	free(*data->input);
	*data->input = ft_strdup(" ");
	buffer = malloc(1);
	if (!buffer)
		return ;
	buffer[0] = '\0';
	while (!ft_is_in(limiter, buffer) && g_sig_status != 4)
	{
		free(buffer);
		if (!get_buffer(&buffer, &nb_line, data, NULL))
			return ;
		*data->input = ft_strjoin_and_free(*data->input,
				ft_substr(buffer, 0, ft_strlen(buffer) - 1));
	}
	free(buffer);
}
