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

// void	handle_word(char *input, t_token **tokens, int *i)
// {
// 	int		start;
// 	char	quote;
// 	char	*word;

// 	start = *i;
// 	if (input[*i] == '&')
// 		(*i)++;
// 	while (input[*i] && input[*i] != ' ' && input[*i] != '|'
// 		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '&'
// 		&& input[*i] != '(' && input[*i] != ')')
// 	{
// 		quote = ' ';
// 		if (input[*i] == '\'' || input[*i] == '\"')
// 			quote = input[(*i)++];
// 		while (input[*i] && input[*i] != '|' && input[*i] != '<'
// 			&& input[*i] != '>' && input[*i] != '&' && input[*i] != '('
// 			&& input[*i] != ')' && input[*i] != quote)
// 			(*i)++;
// 		if (input[*i] == quote && quote != ' ')
// 			(*i)++;
// 	}
// 	word = ft_substr(input, start, *i - start);
// 	if (!word)
// 		return ;
// 	return (add_token(tokens, new_token(TOKEN_WORD, word)), free(word));
// }

void	create_word(char *input, t_token **tokens, int i, int start)
{
	char	*word;

	word = ft_substr(input, start, i - start);
	if (!word)
		return ;
	add_token(tokens, new_token(TOKEN_WORD, word));
	free(word);
}

void	handle_word(char *input, t_token **tokens, int *i)
{
	int		start;
	char	quote;

	start = *i;
	if (input[*i] == '&')
		(*i)++;
	while (input[*i] && input[*i] != ' ' && input[*i] != '|' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '&' && input[*i] != '('
		&& input[*i] != ')')
	{
		quote = ' ';
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			quote = input[(*i)++];
			while (input[*i] && input[*i] != quote)
				(*i)++;
		}
		else
		{
			while (input[*i] && input[*i] != '|' && input[*i] != '<'
				&& input[*i] != '>' && input[*i] != '&' && input[*i] != '('
				&& input[*i] != ')' && input[*i] != quote)
				(*i)++;
		}
		if (input[*i] == quote && quote != ' ')
			(*i)++;
	}
	create_word(input, tokens, *i, start);
}

t_token	*lexer2(char **input, char **env)
{
	t_token	*tokens;
	int		i;

	handle_quotes(input, env);
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
	return (tokens);
}

t_token	*lexer(char **input, char **env)
{
	t_token	*tokens;
	t_token	*end;

	tokens = lexer2(input, env);
	if (!syntax_analyzer(tokens))
	{
		free_token(tokens);
		return (NULL);
	}
	end = handle_last_pipe_op(*input, tokens, env);
	if (end)
		ft_tokadd_back(&tokens, end);
	return (tokens);
}
