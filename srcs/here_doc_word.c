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

//ce bloc
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

// int	ft_is_in(char c, char *str)
// {
// 	int	i;

// 	if (!str)
// 		return (0);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == c)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }






// char	*here_doc_word(char limiter, char **env)
// {
// 	char	*input;
// 	char	*word;

// 	word = ft_strdup("");
// 	if (!word)
// 		return (perror("malloc"), NULL);
// 	while (!ft_isin(limiter, word))
// 	{
// 		input = readline("> ");
// 		if (!input)
// 		{
// 			error_heredocword(limiter, env);
// 			return (word);
// 		}
// 		word = ft_strjoin_and_free(word, input);
// 	}
// 	return (word);
// }





// a partir de la
// char	**reading_lines_word(char **history, char **word, char limiter,
// 		char **env)
// {
// 	char		*input;
// 	int			j;

// 	j = 1;
// 	input = NULL;
// 	while (word && !ft_is_in(limiter, *word))
// 	{
// 		input = readline("> ");
// 		if (!input || status == 130)
// 			return (error_heredocword(limiter, env), NULL);
// 		input = expand_string_heredoc(input, env);
// 		history = ft_realloc(history, sizeof(char *) * (j + 2));
// 		if (!history)
// 			return (NULL);
// 		history[j++] = ft_strdup(input);
// 		*word = ft_strjoin_and_free(*word, input);
// 		if (word && !ft_is_in(limiter, *word))
// 			*word = ft_strjoin_and_free(*word, ft_strdup("\n"));
// 	}
// 	history[j] = NULL;
// 	return (history);
// }

// char	*here_doc_word(char *input_beginning, char limiter, char **env)
// {
// 	char	*input;
// 	char	*word;
// 	char	**history;

// 	word = NULL;
// 	history = ft_calloc(2, sizeof(char *) * 2);
// 	if (!history)
// 		return (NULL);
// 	history[0] = ft_strdup(input_beginning);
// 	free(input_beginning);
// 	history[1] = NULL;
// 	// limiter = clean_limiter(limiter, env);
// 	// if (limiter && !*limiter)
// 	// 	return (here_doc_empty_limiter(env));
// 	history = reading_lines_word(history, &word, limiter, env);
// 	if (!history)
// 		return (NULL);
// 	input = ft_strjoin_sep_realloc(history, '\n');
// 	add_history(input);
// 	return (status = 0, word);
// }





//celui de clement
int	ft_is_in(char c, char *str)
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
	while (!ft_is_in(limiter, buffer) && g_sig_status != 4)
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


//hadle last pipe de clement
// void	handle_last_pipe(char **input, int *status, char **env)
// {
// 	char	*tmp;
// 	int		i;

// 	if (!(*input))
// 		return ;
// 	if (consecutive_pipe(*input) || only_one_pipe(*input))
// 		return ;
// 	i = ft_strlen(*input) - 1;
// 	while ((*input)[i] && (*input)[i] == ' ')
// 		i--;
// 	if (i >= 0 && (*input)[i] == '|')
// 	{
// 		tmp = here_doc_word('\n', status, env);
// 		if (!tmp)
// 			return ;
// 		*input = ft_strjoin_and_free(*input,
// 				ft_strjoin_and_free(ft_strdup(" "), tmp));
// 		handle_last_pipe(input, status, env);
// 	}
// 	if (g_sig_status == 4)
// 		add_history(*input);
// }
