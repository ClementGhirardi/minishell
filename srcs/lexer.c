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

	if ((input[i] == '\'' || input[i] == '"')
		&& input[start] == input[i]
		&& i - start == 2)
		word = ft_strdup("");
	else
		word = ft_substr(input, start, i - start);
	if (!word)
		return ;
	add_token(tokens, new_token(TOKEN_WORD, word));
	free(word);
}

// void	create_word(char *input, t_token **tokens, int i, int start)
// {
// 	char	*word;

// 	word = ft_substr(input, start, i - start);
// 	if (!word)
// 		return ;
// 	add_token(tokens, new_token(TOKEN_WORD, word));
// 	free(word);
// }

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
		while (input[*i] && input[*i] != quote && input[*i] != '\n')
		{
			if (quote == ' '
				&& (input[*i] == '|' || input[*i] == '<' || input[*i] == '>'
				|| input[*i] == '&' || input[*i] == '(' || input[*i] == ')'))
				break ;
			if (quote == ' ' && (input[*i] == '\'' || input[*i] == '\"'))
				quote = input[*i];
			else if (quote != ' ' && (input[*i] == quote))
				quote = ' ';
			(*i)++;
		}
		if (input[*i] == quote && quote != ' ')
			(*i)++;
	}
	create_word(input, tokens, *i, start);
}

// void	handle_word(char *input, t_token **tokens, int *i)
// {
// 	int		start;
// 	char	quote;

// 	start = *i;
// 	if (input[*i] == '&')
// 		(*i)++;
// 	while (input[*i] && input[*i] != ' ' && input[*i] != '|' && input[*i] != '<'
// 		&& input[*i] != '>' && input[*i] != '&' && input[*i] != '('
// 		&& input[*i] != ')')
// 	{
// 		quote = ' ';
// 		if (input[*i] == '\'' || input[*i] == '\"')
// 		{
// 			quote = input[(*i)++];
// 			while (input[*i] && input[*i] != quote)
// 				(*i)++;
// 		}
// 		else
// 		{
// 			while (input[*i] && input[*i] != '|' && input[*i] != '<'
// 				&& input[*i] != '>' && input[*i] != '&' && input[*i] != '('
// 				&& input[*i] != ')' && input[*i] != quote)
// 				(*i)++;
// 		}
// 		if (input[*i] == quote && quote != ' ')
// 			(*i)++;
// 	}
// 	create_word(input, tokens, *i, start);
// }

t_token	*lexer2(char **input, int *status, char **env)
{
	t_token	*tokens;
	int		i;

	// handle_quotes(input, status, env);
	//add_history(*input);
	(void)status;
	(void)env;
	if (handle_quotes(input))
		return (NULL);
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

t_token	*lexer(char **input, int *status, char **env)
{
	t_token	*tokens;
	t_token	*end;

	tokens = lexer2(input, status, env);
	if (!syntax_analyzer(tokens, status))
	{
		free_token(tokens);
		return (NULL);
	}
	end = handle_last_pipe_op(*input, tokens, status, env);
	if (end)
		ft_tokadd_back(&tokens, end);
	return (tokens);
}
