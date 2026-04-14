/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:26:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/14 14:40:11 by clement-ghi      ###   ########.fr       */
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

char	*here_doc_word(char limiter)
{
	char	*word;
	char	*buffer;

	word = ft_strdup("");
	if (!word)
		return (perror("malloc"), NULL);
	write(1, "> ", 2);
	buffer = get_next_line(STDIN_FILENO);
	if (!buffer)
		return (free(word), perror("malloc"), NULL);
	while (!ft_isin(limiter, buffer))
	{
		word = ft_strjoin_and_free(word, buffer);
		write(1, "> ", 2);
		buffer = get_next_line(STDIN_FILENO);
		if (!buffer)
			return (free(word), perror("malloc"), NULL);
	}
	word = ft_strjoin_and_free(word,
			ft_substr(buffer, 0, ft_strlen(buffer) - 1));
	if (buffer)
		free(buffer);
	return (word);
}
