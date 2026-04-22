/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:26:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 12:50:01 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_safe_strlen(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

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

// char	*here_doc_word(char limiter)
// {
// 	char	*word;
// 	char	*buffer;

// 	word = ft_strdup("");
// 	if (!word)
// 		return (perror("malloc"), NULL);
// 	write(1, "> ", 2);
// 	buffer = get_next_line(STDIN_FILENO);
// 	if (!buffer)
// 		return (free(word), perror("malloc"), NULL);
// 	while (!ft_isin(limiter, buffer))
// 	{
// 		word = ft_strjoin_and_free(word, buffer);
// 		write(1, "> ", 2);
// 		buffer = get_next_line(STDIN_FILENO);
// 		if (!buffer)
// 			return (free(word), perror("malloc"), NULL);
// 	}
// 	word = ft_strjoin_and_free(word,
// 			ft_substr(buffer, 0, ft_strlen(buffer) - 1));
// 	if (buffer)
// 		free(buffer);
// 	return (word);
// }


char	*here_doc_word(char limiter, char **env, int status)
{
	char	*input;
	char	*word;

	input = ft_strdup("");
	word = ft_strdup("");
	if (!input || !word)
		return (perror("malloc"), NULL);
	while (!ft_isin(limiter, word))
	{
		input = readline("> ");
		if (!input)
		{
			error_heredocword(limiter, env, status);
			free(word);
			break ;
		}
		// ligne dessous inclut la quote
		word = ft_strjoin_and_free(word, input);
		// word = ft_strjoinsep_free(word, input, '\n');
	}
	return (word);
}
