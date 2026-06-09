/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 15:06:25 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/05 11:32:56 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell.h"

int	handle_quotes(char **input)
{
	int		i;
	char	quote;

	i = 0;
	if (!input || !*input || !**input)
		return (1);
	while ((*input)[i])
	{
		if ((*input)[i] == '\'' || (*input)[i] == '"')
		{
			quote = (*input)[i++];
			while ((*input)[i] && (*input)[i] != quote)
				i++;
			if (!(*input)[i])
				return (1);
		}
		i++;
	}
	return (0);
}

void	handle_pipe(char *input, t_token **tokens, int *i)
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

void	handle_redir(char *input, t_token **tokens, int *i, int dir)
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

// void	create_word(char *input, t_token **tokens, int i, int start)
// {
// 	char	*word;

// 	if ((input[i] == '\'' || input[i] == '"')
// 		&& input[start] == input[i]
// 		&& i - start == 2)
// 		word = ft_strdup("");
// 	else
// 		word = ft_substr(input, start, i - start);
// 	if (!word)
// 		return ;
// 	add_token(tokens, new_token(TOKEN_WORD, word));
// 	free(word);
// }

// void	handle_word(char *input, t_token **tokens, int *i)
// {
// 	int		start;
// 	char	quote;

// 	start = *i;
// 	if (input[*i] == '&')
// 		(*i)++;
// 	while (input[*i] && !ft_is_in(input[*i], " |<>&"))
// 	{
// 		quote = ' ';
// 		while (input[*i] && input[*i] != quote)
// 		{
// 			if (quote == ' ' && ft_is_in(input[*i], "|<>&"))
// 				break ;
// 			if (quote == ' ' && ft_is_in(input[*i], "\'\""))
// 				quote = input[*i];
// 			else if (quote != ' ' && (input[*i] == quote))
// 				quote = ' ';
// 			if (input[*i] == '\n')
// 				return (create_other_lines(input, tokens, i, start));
// 			(*i)++;
// 		}
// 		if (input[*i] == quote && quote != ' ')
// 			(*i)++;
// 	}
// 	create_word(input, tokens, *i, start);
// }
