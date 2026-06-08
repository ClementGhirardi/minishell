/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 15:06:25 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 17:57:06 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_bracket(char *input, t_token **tokens, int *i)
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

void	handle_and(char *input, t_token **tokens, int *i)
{
	if (input[*i + 1] == '&')
	{
		add_token(tokens, new_token(TOKEN_AND, "&&"));
		*i += 2;
		return ;
	}
	handle_word(input, tokens, i);
}

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

// void	handle_word(char *input, t_token **tokens, int *i)
// {
// 	int		start;
// 	char	quote;

// 	start = *i;
// 	while (input[*i] && input[*i] != ' ' && input[*i] != '|'
// 		&& input[*i] != '<' && input[*i] != '>' && !(input[*i] == '&'
// 			&& input[*i + 1] == '&') && input[*i] != '(' && input[*i] != ')')
// 	{
// 		quote = ' ';
// 		while (input[*i] && input[*i] != quote && input[*i] != '\n')
// 		{
// 			if (quote == ' ' && (input[*i] == '|' || input[*i] == '<'
// 					|| input[*i] == '>' || (input[*i] == '&' && input[*i + 1]
// 						== '&') || input[*i] == '(' || input[*i] == ')'))
// 				break ;
// 			if (quote == ' ' && (input[*i] == '\'' || input[*i] == '\"'))
// 				quote = input[*i];
// 			else if (quote != ' ' && (input[*i] == quote))
// 				quote = ' ';
// 			(*i)++;
// 		}
// 		if (input[*i] == quote && quote != ' ')
// 			(*i)++;
// 	}
// 	create_word(input, tokens, *i, start);
// }

void	handle_word(char *input, t_token **tokens, int *i)
{
	int		start;
	char	quote;

	start = *i;
	if (input[*i] == '&')
		(*i)++;
	while (input[*i] && !ft_is_in(input[*i], " |<>&"))
	{
		quote = ' ';
		while (input[*i] && input[*i] != quote)
		{
			if (quote == ' ' && ft_is_in(input[*i], "|<>&"))
				break ;
			if (quote == ' ' && ft_is_in(input[*i], "\'\""))
				quote = input[*i];
			else if (quote != ' ' && (input[*i] == quote))
				quote = ' ';
			if (input[*i] == '\n')
				return (create_other_lines(input, tokens, i, start));
			(*i)++;
		}
		if (input[*i] == quote && quote != ' ')
			(*i)++;
	}
	create_word(input, tokens, *i, start);
}
