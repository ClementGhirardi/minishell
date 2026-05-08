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

char	*ft_strdup_no_empty_quotes(char *str);
int		count_empty_quotes(char *str);

int	here_doc_empty_limiter(char **env)
{
	char		*input;
	int			fd[2];
	static int	i;
	int			first_line;

	first_line = ++i;
	pipe(fd);
	input = ft_strdup("\n");
	while (input && *input)
	{
		ft_putstr_fd(input, fd[1]);
		free(input);
		input = readline("> ");
		if (!input)
		{
			error_heredoc(first_line, "");
			close(fd[1]);
			//i--;
			break ;
		}
		input = expand_string_heredoc(input, env);
		i++;
	}
	return (i++, free(input), close(fd[1]), status = 0, fd[0]);
}

void	*ctrld_heredoc(char **history, char *limiter, int first_line, int fd1, int j)
{
	int	i;

	i = 0;
	error_heredoc(first_line, limiter);
	close(fd1);
	history[j] = NULL;
	return (history);
}

char	**reading_lines(char **history, char *limiter, int *fd, char **env)
{
	char		*input;
	int			j;
	int			first_line;
	static int	i;

	first_line = ++i;
	j = 1;
	input = ft_strdup("");
	while (limiter && ft_strcmp(input, limiter))
	{
		ft_putstr_fd(input, fd[1]);
		free(input);
		input = readline("> ");
		if (!input || status == 130)
			return (ctrld_heredoc(history, limiter, first_line, fd[1], j));
		input = expand_string_heredoc(input, env);
		history = ft_realloc(history, sizeof(char *) * (j + 2));
		if (!history)
			return (NULL);
		history[j++] = ft_strdup(input);
		i++;
	}
	history[j] = NULL;
	return (i++, history);
}

char	*clean_limiter(char *limiter, char **env)
{
	char	*clean_limiter;
	char	*tmp;

	tmp = expand_string(ft_strdup(limiter), env);
	clean_limiter = ft_strjoin_and_free(ft_strdup(""),
			ft_strdup_no_empty_quotes(tmp));
	clean_limiter = ft_strjoin_and_free(ft_strdup(""), clean_limiter);
	free(tmp);
	free(limiter);
	return (clean_limiter);
}

int	here_doc(char *limiter, char **env)
{
	char		*input;
	char		**history;
	int			fd[2];

	pipe(fd);
	history = ft_calloc(2, sizeof(char *) * 2);
	if (!history || !limiter)
		return (1);
	history[0] = ft_strjoin("<< ", limiter);
	history[1] = NULL;
	limiter = clean_limiter(limiter, env);
	if (limiter && !*limiter)
		return (here_doc_empty_limiter(env));
	history = reading_lines(history, limiter, fd, env);
	if (!history)
		return (1);
	input = ft_strjoin_sep_realloc(history, '\n');
	add_history(input);
	return (status = 0, close(fd[1]), free(limiter), fd[0]);
}
