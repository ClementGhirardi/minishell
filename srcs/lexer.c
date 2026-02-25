/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:53:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/25 19:03:27 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_pipe(t_token **tokens, int *i)
{
	add_token(tokens, new_token(TOKEN_PIPE, "|"));
	(*i)++;
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
		add_token(tokens, new_token(TOKEN_APPEND, "<<"));
		*i += 2;
	}
	else
	{
		add_token(tokens, new_token(TOKEN_REDIR_OUT, "<"));
		(*i)++;
	}
}

static void	handle_quotes(char *input, t_token **tokens, int *i)
{
	char			quote;
	int				start;
	char			*word;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	word = ft_substr(input, start, *i - start);
	if (quote == '\'')
		add_token(tokens, new_token(TOKEN_WORD_SQUOTE, word));
	else
		add_token(tokens, new_token(TOKEN_WORD_DQUOTE, word));
	free(word);
	if (input[*i] == quote)
		(*i)++;
}

static void	handle_word(char *input, t_token **tokens, int *i)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i]
		&& input[*i] != ' '
		&& input[*i] != '|'
		&& input[*i] != '<'
		&& input[*i] != '>'
		&& input[*i] != '\''
		&& input[*i] != '"')
		(*i)++;
	word = ft_substr(input, start, *i - start);
	add_token(tokens, new_token(TOKEN_WORD, word));
	free(word);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == ' ')
			i++;
		else if (input[i] == '|')
			handle_pipe(&tokens, &i);
		else if (input[i] == '>')
			handle_redir(input, &tokens, &i, 1);
		else if (input[i] == '<')
			handle_redir(input, &tokens, &i, 0);
		else if (input[i] == '\'' || input[i] == '"')
			handle_quotes(input, &tokens, &i);
		else
			handle_word(input, &tokens, &i);
	}
	return (tokens);
}
