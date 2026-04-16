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

// static void	handle_quotes(char **input)
// {
// 	int	quote[2];
// 	int	i;

// 	if (!(*input))
// 		return ;
// 	quote[0] = 0;
// 	quote[1] = 0;
// 	i = 0;
// 	while ((*input)[i])
// 	{
// 		if ((*input)[i] == '\'')
// 			quote[0] = (quote[0] + 1) % 2;
// 		if ((*input)[i] == '\"')
// 			quote[1] = (quote[1] + 1) % 2;
// 		i++;
// 	}
// 	if (quote[0])
// 		*input = ft_strjoin_and_free(*input,
// 				ft_strjoin_and_free(ft_strdup("\n"), here_doc_word('\'')));
// 	else if (quote[1])
// 		*input = ft_strjoin_and_free(*input,
// 				ft_strjoin_and_free(ft_strdup("\n"), here_doc_word('\"')));
// 	else
// 		return ;
// 	lexer(input);
// }


static void	handle_quotes(char **input)
{
	int		i;
	char	quote;

	if (!(*input))
		return ;
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == '\'' || (*input)[i] == '\"')
		{
			quote = (*input)[i++];
			while ((*input)[i] && (*input)[i] != quote)
				i++;
			if (!(*input)[i])
				break ;
		}
		i++;
		quote = '\0';
	}
	if (quote)
		*input = ft_strjoin_and_free(*input,
				ft_strjoin_and_free(ft_strdup("\n"), here_doc_word(quote)));
	else
		return ;
	lexer(input);
}


// static char	*handle_empty_quotes(char *input)
// {
// 	char	*result;
// 	int		i;
// 	char	quote;

// 	result = NULL;
// 	i = 0;
// 	if (!input)
// 		return (NULL);
// 	// if (!input[i])
// 	// 	return (ft_printf("ok\n"), ft_strdup(""));
// 	ft_printf("%s\n", input);
// 	while (input[i])
// 	{
// 		if (input[i] == '\'' || input[i] == '"')
// 		{
// 			quote = input[i];
// 			if (input[i + 1] == quote)
// 				i += 2;
// 		}
// 		else
// 		{
// 			result = ft_strjoin_char_free(result, input[i]);
// 			i++;
// 		}
// 	}
// 	if (!result)
// 		return (ft_strdup(""));
// 	//free_array(input);
// 	return (result);
// }

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
	while (input[*i] && input[*i] != ' ' && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '&'
		&& input[*i] != '(' && input[*i] != ')')
	{
		quote = ' ';
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			quote = input[*i];
			(*i)++;
		}
		while (input[*i] && input[*i] != '|'
			&& input[*i] != '<' && input[*i] != '>'
			&& input[*i] != '&' && input[*i] != '('
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

t_token	*lexer2(char **input)
{
	t_token	*tokens;
	int		i;

	handle_quotes(input);
	add_history(*input);
	//*input = handle_empty_quotes(input);
	tokens = NULL;
	// if (input && !input[0])
	// 	add_token(&tokens, new_token(TOKEN_WORD, ft_strdup("")));
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

t_token	*lexer(char **input)
{
	// t_token	*tokens;
	// int		i;

	handle_last_pipe(input);
	return (lexer2(input));
	// handle_quotes(input);
	// add_history(*input);
	// tokens = NULL;
	// i = 0;
	// while ((*input)[i])
	// {
	// 	if ((*input)[i] == ' ')
	// 		i++;
	// 	else if ((*input)[i] == '&')
	// 		handle_and(*input, &tokens, &i);
	// 	else if ((*input)[i] == '|')
	// 		handle_pipe(*input, &tokens, &i);
	// 	else if ((*input)[i] == '>')
	// 		handle_redir(*input, &tokens, &i, 1);
	// 	else if ((*input)[i] == '<')
	// 		handle_redir(*input, &tokens, &i, 0);
	// 	else if ((*input)[i] == '(' || (*input)[i] == ')')
	// 		handle_bracket(*input, &tokens, &i);
	// 	else
	// 		handle_word(*input, &tokens, &i);
	// }
	// return (check_brackets(tokens));
}
