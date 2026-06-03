/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:53:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/20 11:58:56 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_quotes(char **input)
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

t_token	*lexer2(char **input)
{
	t_token	*tokens;
	int		i;

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

t_token	*lexer(t_data *data, char **input)
{
	t_token	*tokens;

	handle_end_with_pipe(input, data);
	if (!input || !*input || !**input)
		return (NULL);
	if (handle_quotes(input))
		return (NULL);
	if (!no_space_bracket(*input, data->status))
		return (NULL);
	tokens = lexer2(input);
	if (!syntax_analyzer(tokens, data->status))
		return (free_token(tokens), NULL);
	return (tokens);
}
