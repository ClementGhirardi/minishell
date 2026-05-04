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

int	isempty(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

char	*here_doc_pipe_op(char **env)
{
	char	*input;

	input = readline("> ");
	if (!input)
		error_heredocword(0, env);
	while (input && isempty(input))
	{
		free(input);
		input = readline("> ");
		if (!input)
			error_heredocword(0, env);
	}
	return (input);
}

static char	*expand_string_heredoc(char *str, char **env)
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
			var = extract_var_name(str, &i, env);
		else
			var = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, var);
	}
	return (free(str), result);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (300);
	while (s1[i] && s2[i])
	{
		if (s1[i] - s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

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

int	here_doc(char *limiter, char **env)
{
	char		*input;
	char		*clean_limiter;
	int			fd[2];
	static int	i;
	int			first_line;

	first_line = ++i;
	pipe(fd);
	input = expand_string(ft_strdup(limiter), env);
	clean_limiter = ft_strjoin_and_free(ft_strdup(""), ft_strdup_no_empty_quotes(input));
	clean_limiter = ft_strjoin_and_free(ft_strdup(""), clean_limiter);
	free(input);
	if (!clean_limiter)
		return (close(fd[1]), fd[0]);
	if (!*clean_limiter)
		return (close(fd[0]), close(fd[1]), free(clean_limiter), here_doc_empty_limiter(env));
	input = ft_strdup("");
	while (ft_strcmp(input, clean_limiter))
	{
		ft_putstr_fd(input, fd[1]);
		free(input);
		input = readline("> ");
		//ft_printf("status = %d\n", status);
		if (!input || status == 130)
		{
			error_heredoc(first_line, clean_limiter);
			close(fd[1]);
			//i--;
			break ;
		}
		input = expand_string_heredoc(input, env);
		i++;
	}
	// if (status == 130)
	// {
	// 	error_heredoc(first_line, clean_limiter);
	// 	close(fd[1]);
	// }
	return (status = 0, i++, close(fd[1]), free(clean_limiter),
		free(input), fd[0]);
}
