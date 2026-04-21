/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:51:27 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/30 13:55:15 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*here_doc_pipe(char limiter, char **env, int status)
{
	char	*input;
	char	*word;

	input = ft_strdup("");
	word = ft_strdup("");
	if (!input || !word)
		return (perror("malloc"), NULL);
	input = readline("> ");
	if (!input)
	{
		error_heredocword(limiter, env, status);
		free(word);
	}
	//word = ft_strjoinsep_free(word, input, '\n');
	//return (word);
	return (input);
}

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

// int	here_doc(char *limiter, int status, char **env)
// {
// 	char	*buffer;
// 	int		size;
// 	int		fd[2];

// 	pipe(fd);
// 	size = ft_strlen(limiter);
// 	write(1, "> ", 2);
// 	buffer = expand_string(get_next_line(STDIN_FILENO), status, env);
// 	if (!buffer)
// 		perror("gnl");
// 	while (ft_strncmp(buffer, limiter, size))
// 	{
// 		ft_putstr_fd(buffer, fd[1]);
// 		free(buffer);
// 		write(1, "> ", 2);
// 		buffer = expand_string(get_next_line(STDIN_FILENO), status, env);
// 		if (!buffer)
// 			perror("gnl");
// 	}
// 	free(buffer);
// 	close(fd[1]);
// 	return (fd[0]);
// }

int	here_doc(char *limiter, int status, char **env)
{
	char	*input;
	int		size;
	int		fd[2];
	static int	i;

	pipe(fd);
	size = ft_strlen(limiter);
	input = ft_strdup("");
	while (ft_strncmp(input, limiter, size) && ++i)
	{
		ft_putstr_fd(input, fd[1]);
		free(input);
		input = readline("> ");
		if (!input)
		{
			error_heredoc(i, limiter);
			close(fd[1]);
			break ;
		}
		input = expand_string(input, status, env);
	}
	free(input);
	close(fd[1]);
	return (fd[0]);
}

