/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbarth <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 11:35:12 by adbarth           #+#    #+#             */
/*   Updated: 2026/04/17 11:35:14 by adbarth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	handle_word(char *input, t_token **tokens, int *i)
{
	int		start;
	char	quote;
	char	*word;

	start = *i;
	if (input[*i] == '&')
		(*i)++;
	while (input[*i] && input[*i] != ' ' && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '&'
		&& input[*i] != '(' && input[*i] != ')')
	{
		quote = ' ';
		if (input[*i] == '\'' || input[*i] == '\"')
			quote = input[(*i)++];
		while (input[*i] && input[*i] != '|' && input[*i] != '<'
			&& input[*i] != '>' && input[*i] != '&' && input[*i] != '('
			&& input[*i] != ')' && input[*i] != quote)
			(*i)++;
		if (input[*i] == quote && quote != ' ')
			(*i)++;
	}
	word = ft_substr(input, start, *i - start);
	if (!word)
		return ;
	return (add_token(tokens, new_token(TOKEN_WORD, word)), free(word));
}

static t_token	*check_brackets(t_token	*tokens)
{
	t_token	*tmp;
	int		o_brack;
	int		c_brack;

	o_brack = 0;
	c_brack = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_O_BRACK)
			o_brack++;
		if (tmp->type == TOKEN_C_BRACK)
			c_brack++;
		if (c_brack > o_brack)
			return (syntax_error(")"));
		tmp = tmp->next;
	}
	if (o_brack == c_brack)
		return (tokens);
	else
		return (syntax_error("("));
}

t_token	*lexer2(char **input, char **env, int status)
{
	t_token	*tokens;
	int		i;

	handle_quotes(input, env, status);
	add_history(*input);
	tokens = NULL;
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == ' ')
			i++;
		else if ((*input)[i] == '&')
			handle_and(*input, &tokens, &i);
		else if ((*input)[i] == '|')
			handle_pipe(*input, &tokens, &i);
		else if ((*input)[i] == '>')
			handle_redir(*input, &tokens, &i, 1);
		else if ((*input)[i] == '<')
			handle_redir(*input, &tokens, &i, 0);
		else if ((*input)[i] == '(' || (*input)[i] == ')')
			handle_bracket(*input, &tokens, &i);
		else
			handle_word(*input, &tokens, &i);
	}
	return (check_brackets(tokens));
}

t_token	*lexer(char **input, char **env, int status)
{
	handle_last_pipe(input, env, status);
	return (lexer2(input, env, status));
}
