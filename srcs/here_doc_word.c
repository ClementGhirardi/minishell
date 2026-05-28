/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:26:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/13 15:21:25 by cghirard         ###   ########.fr       */
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

void	here_doc_word(char limiter, t_data *data)
{
	char	*buffer;
	int		nb_line;

	nb_line = -1;
	if (g_sig_status == 4)
		return ;
	add_history(*data->input);
	*data->input = ft_strjoin_and_free(*data->input, ft_strdup(" "));
	if (!get_buffer(&buffer, &nb_line, data))
	{
		free(*data->input);
		return ;
	}
		// return (free(*data->input), NULL);
	while (!ft_isin(limiter, buffer) && g_sig_status != 4)
	{
		*data->input = ft_strjoin_and_free(*data->input, buffer);
		if (!get_buffer(&buffer, &nb_line, data))
		{
			return ;
		}
			//return (*data->input);
	}
	*data->input = ft_strjoin_and_free(*data->input,
			ft_substr(buffer, 0, ft_strlen(buffer) - 1));
	free(buffer);
	// if (g_sig_status == 4)
		// return (free(buffer), *data->input);
	// return (free(buffer), *data->input);
}

// char	*here_doc_word(char limiter, int *status, char **env)
// {
// 	char	*word;
// 	char	*buffer;
// 	int		nb_line;

// 	nb_line = -1;
// 	if (g_sig_status == 4)
// 		return (NULL);
// 	word = ft_strdup("");
// 	if (!word)
// 		return (NULL);
// 	if (!get_buffer(&buffer, &nb_line, status, env))
// 		return (free(word), NULL);
// 	while (!ft_is_in(limiter, buffer) && g_sig_status != 4)
// 	{
// 		word = ft_strjoin_and_free(word, buffer);
// 		if (!get_buffer(&buffer, &nb_line, status, env))
// 			return (word);
// 	}
// 	word = ft_strjoin_and_free(word,
// 			ft_substr(buffer, 0, ft_strlen(buffer) - 1));
// 	if (g_sig_status == 4)
// 		return (free(buffer), word);
// 	return (free(buffer), word);
// }

// char	*here_doc_word(char **input, char limiter, int *status, char **env)
// {
// 	char	*buffer;
// 	int		nb_line;

// 	nb_line = -1;
// 	if (g_sig_status == 4)
// 		return (NULL);
// 	add_history(*input);
// 	*input = ft_strjoin_and_free(*input, ft_strdup(" "));
// 	if (!get_buffer(&buffer, &nb_line, status, env))
// 		return (NULL);
// 	while (!ft_isin(limiter, buffer) && g_sig_status != 4)
// 	{
// 		*input = ft_strjoin_and_free(*input, buffer);
// 		if (!get_buffer(&buffer, &nb_line, status, env))
// 			return (*input);
// 	}
// 	*input = ft_strjoin_and_free(*input,
// 			ft_substr(buffer, 0, ft_strlen(buffer) - 1));
// 	if (g_sig_status == 4)
// 		return (free(buffer), *input);
// 	return (free(buffer), *input);
// }
