/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:26:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/05 15:31:36 by clement-ghi      ###   ########.fr       */
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

char	*here_doc_word(char limiter, int *status, char **env)
{
	char	*word;
	char	*buffer;
	int		nb_line;

	nb_line = -1;
	if (g_sig_status == 4)
		return (NULL);
	word = ft_strdup("");
	if (!word)
		return (NULL);
	if (!get_buffer(&buffer, &nb_line, status, env))
		return (free(word), NULL);
	while (!ft_isin(limiter, buffer) && g_sig_status != 4)
	{
		word = ft_strjoin_and_free(word, buffer);
		if (!get_buffer(&buffer, &nb_line, status, env))
			return (word);
	}
	word = ft_strjoin_and_free(word,
			ft_substr(buffer, 0, ft_strlen(buffer) - 1));
	if (g_sig_status == 4)
		return (free(buffer), word);
	return (free(buffer), word);
}
