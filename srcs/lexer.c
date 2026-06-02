/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:53:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 13:45:22 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_quotes(char **input)
{
	int	quote[2];
	int	i;

	if (!(*input))
		return (1);
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
	if (quote[0] || quote[1])
		return (1);
	return (0);
}

void	handle_pipe(t_token **tokens, int *i)
{
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

t_token	*lexer2(char **input, int *status, char **env)
{
	t_token	*tokens;
	int		i;

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
		else if ((*input)[i] == '|')
			handle_pipe(&tokens, &i);
		else if ((*input)[i] == '>')
			handle_redir(*input, &tokens, &i, 1);
		else if ((*input)[i] == '<')
			handle_redir(*input, &tokens, &i, 0);
		else
			handle_word(*input, &tokens, &i);
	}
	return (tokens);
}

t_token	*lexer(t_data *data, char **input)
{
	t_token	*tokens;

	handle_last_pipe(input, data);
	if (!input || !*input || !**input)
		return (NULL);
	if (handle_quotes(input))
		return (NULL);
	tokens = lexer2(input, data->status, data->env);
	if (!syntax_analyzer(tokens, data->status))
		return (free_token(tokens), NULL);
	return (tokens);
}
