/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:53:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/15 15:43:33 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_last_pipe(char **input)
{
	int	i;

	if (!(*input))
		return ;
	i = ft_strlen(*input) - 1;
	while ((*input)[i] && (*input)[i] == ' ')
		i--;
	if (i >= 0 && (*input)[i] == '|')
		*input = ft_strjoin_and_free(*input,
				ft_strjoin_and_free(ft_strdup(" "), here_doc_word('\n')));
	else
		return ;
	lexer(input);
}

static void	handle_quotes(char **input)
{
	int	quote[2];
	int	i;

	if (!(*input))
		return ;
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
	if (quote[0])
		*input = ft_strjoin_and_free(*input,
				ft_strjoin_and_free(ft_strdup("\n"), here_doc_word('\'')));
	else if (quote[1])
		*input = ft_strjoin_and_free(*input,
				ft_strjoin_and_free(ft_strdup("\n"), here_doc_word('\"')));
	else
		return ;
	lexer(input);
}

// static void	handle_empty_quotes(char **input)
// {
// 	char	*result;
// 	size_t	i;

// 	if (!(*input) && ft_strncmp(*input, "", ft_strlen(*input)))
// 		return ;
// 	result = NULL;
// 	i = 0;
// 	while ((*input)[i] && (*input)[i + 1])
// 	{
// 		if (((*input)[i] == '\'' || (*input)[i] == '"')
// 			&& ((*input)[i] == (*input)[i + 1]))
// 		{
// 			if (i == 0)
// 				result = ft_substr(*input, i + 2, ft_strlen(*input));
// 			else if (i + 3 < ft_strlen(*input))
// 				result = ft_substr(*input, 0, i);
// 			else
// 				result = ft_strjoin_and_free(ft_substr(*input, 0, i),
// 						ft_substr(*input, i + 2, ft_strlen(*input)));
// 			ft_printf("%s\n", result);
// 			free(*input);
// 			*input = result;
// 			i++;
// 		}
// 		i++;
// 	}
// }

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

static void	handle_word(char *input, t_token **tokens, int *i)
{
	int		start;
	char	quote;
	char	*word;

	start = *i;
	while (input[*i] && input[*i] != ' ')
	{
		quote = ' ';
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			quote = input[*i];
			(*i)++;
		}
		while (input[*i] && input[*i] != '|'
			&& input[*i] != '<' && input[*i] != '>'
			&& input[*i] != quote)
			(*i)++;
		if (input[*i] == quote && quote != ' ')
			(*i)++;
	}
	word = ft_substr(input, start, *i - start);
	if (!word)
		return ;
	return (add_token(tokens, new_token(TOKEN_WORD, word)), free(word));
}

t_token	*lexer(char **input)
{
	t_token	*tokens;
	int		i;

	handle_last_pipe(input);
	handle_quotes(input);
	add_history(*input);
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
