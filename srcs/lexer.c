/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:53:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 13:18:48 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_word(char *input, t_token **tokens, int *i);

static char	*handle_quotes(char *input)
{
	int	squote;
	int	dquote;
	int	i;

	if (!input)
		return (NULL);
	squote = 0;
	dquote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			squote = (squote + 1) % 2;
		if (input[i] == '\"')
			dquote = (dquote + 1) % 2;
		i++;
	}
	if (squote)
		input = ft_strjoin_and_free(input, here_doc_word('\''));
	else if (dquote)
		input = ft_strjoin_and_free(input, here_doc_word('\"'));
	else
		return (input);
	return (handle_quotes(input));
}

static void	handle_pipe(char *input, t_token **tokens, int *i)
{
	if (input[*i + 1] == '|')
	{
		add_token(tokens, new_token(TOKEN_OR, "||"));
		*i += 2;
		return ;
	}
	add_token(tokens, new_token(TOKEN_PIPE, "|"));
	(*i)++;
}

static void	handle_and(char *input, t_token **tokens, int *i)
{
	if (input[*i + 1] == '&')
	{
		add_token(tokens, new_token(TOKEN_AND, "&&"));
		*i += 2;
		return ;
	}
	handle_word(input, tokens, i);
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

static void	handle_bracket(char *input, t_token **tokens, int *i)
{
	if (input[*i] == '(')
	{
		add_token(tokens, new_token(TOKEN_O_BRACK, "("));
		(*i)++;
		return ;
	}
	add_token(tokens, new_token(TOKEN_C_BRACK, ")"));
	(*i)++;
}

static void	handle_word(char *input, t_token **tokens, int *i)
{
	int		start;
	char	quote;
	char	*word;

	start = *i;
	if (input[*i] == '&')
		(*i)++;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		quote = input[*i];
		(*i)++;
	}
	else
		quote = ' ';
	while (input[*i] && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>'
		&& input[*i] != quote && input[*i] != '&'
		&& input[*i] != '(' && input[*i] != ')')
		(*i)++;
	if (input[*i] == quote && quote != ' ')
		(*i)++;
	word = ft_substr(input, start, *i - start);
	if (!word)
		return ;
	return (add_token(tokens, new_token(TOKEN_WORD, word)), free(word));
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	int		i;

	input = handle_quotes(input);
	if (!input)
		return (NULL);
	add_history(input);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == ' ')
			i++;
		else if (input[i] == '|')
			handle_pipe(input, &tokens, &i);
		else if (input[i] == '&')
			handle_and(input, &tokens, &i);
		else if (input[i] == '>')
			handle_redir(input, &tokens, &i, 1);
		else if (input[i] == '<')
			handle_redir(input, &tokens, &i, 0);
		else if (input[i] == '(' || input[i] == ')')
			handle_bracket(input, &tokens, &i);
		else
			handle_word(input, &tokens, &i);
	}
	return (tokens);
}
