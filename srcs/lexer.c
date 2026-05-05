/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:53:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/05 15:48:12 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_quotes(char **input)
{
	int	quote[2];
	int	i;

	if (!(*input))
		return (1);
	quote[0] = 0;
	quote[1] = 0;
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == '\'')
			quote[0] = (quote[0] + 1) % 2;
		if ((*input)[i] == '\"')
			quote[1] = (quote[1] + 1) % 2;
		i++;
	}
	if (quote[0] || quote[1])
		return (1);
	return (0);
}

static void	handle_redir(char *input, t_token **tokens, int *i, int dir)
{
	if (dir == 1)
	{
		if (input[*i + 1] == '>')
		{
			add_token(tokens, new_token(TOKEN_APPEND, ">>"));
			*i += 2;
		}
		else
		{
			add_token(tokens, new_token(TOKEN_REDIR_OUT, ">"));
			(*i)++;
		}
		return ;
	}
	if (input[*i + 1] == '<')
	{
		add_token(tokens, new_token(TOKEN_HEREDOC, "<<"));
		*i += 2;
	}
	else
	{
		add_token(tokens, new_token(TOKEN_REDIR_IN, "<"));
		(*i)++;
	}
}

static int	read_word(char *input, int *i)
{
	int		start;
	char	quote;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\n'
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		quote = ' ';
		while (input[*i] && input[*i] != quote && input[*i] != '\n')
		{
			if (quote == ' '
				&& (input[*i] == '|' || input[*i] == '<' || input[*i] == '>'))
				break ;
			if (quote == ' ' && (input[*i] == '\'' || input[*i] == '\"'))
				quote = input[*i];
			else if (quote != ' ' && (input[*i] == quote))
				quote = ' ';
			(*i)++;
		}
	}
	return (start);
}

static void	handle_word(char *input, t_token **tokens, int *i)
{
	int		start;
	char	*word;

	start = read_word(input, i);
	if (start != *i)
	{
		word = ft_substr(input, start, *i - start);
		add_token(tokens, new_token(TOKEN_WORD, word));
		free(word);
	}
	if (input[*i] == '\n')
		return (start = *i, *i = ft_strlen(input),
			word = ft_substr(input, start + 1, *i - start),
			add_token(tokens, new_token(TOKEN_WORD, word)), free(word));
}

t_token	*lexer(char **input, int *status, char **env)
{
	t_token	*tokens;
	int		i;

	handle_last_pipe(input, status, env);
	if (handle_quotes(input))
		return (NULL);
	tokens = NULL;
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == ' ')
			i++;
		else if ((*input)[i] == '|')
		{
			add_token(&tokens, new_token(TOKEN_PIPE, "|"));
			i++;
		}
		else if ((*input)[i] == '>')
			handle_redir(*input, &tokens, &i, 1);
		else if ((*input)[i] == '<')
			handle_redir(*input, &tokens, &i, 0);
		else
			handle_word(*input, &tokens, &i);
	}
	return (tokens);
}
