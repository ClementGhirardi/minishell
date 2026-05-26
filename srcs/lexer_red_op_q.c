/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_red_op_q.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:53:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 14:13:29 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_word(char *input, t_token **tokens, int *i);

t_token	*handle_last_pipe_op(char *input, t_token *tokens, int *status,
		char **env)
{
	t_token	*tmp;
	t_token	*end_token;
	char	*heredoc_output;

	if (!tokens)
		return (NULL);
	tmp = tokens;
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->type == TOKEN_PIPE
		|| tmp->type == TOKEN_AND
		|| tmp->type == TOKEN_OR)
	{
		heredoc_output = ft_strjoin_and_free(ft_strdup(" "),
				here_doc_pipe_op(ft_strdup(input), status, env));
		end_token = lexer(&heredoc_output, status, env);
		free(heredoc_output);
		return (end_token);
	}
	return (NULL);
}

// void	handle_quotes(char **input, int *status, char **env)
// {
// 	int		i;
// 	char	quote;

// 	(void)status;
// 	if (!input || !(*input))
// 		return ;
// 	i = -1;
// 	while ((*input)[++i])
// 	{
// 		if ((*input)[i] == '\'' || (*input)[i] == '\"')
// 		{
// 			quote = (*input)[i++];
// 			while ((*input)[i] && (*input)[i] != quote)
// 				i++;
// 			if (!(*input)[i])
// 				break ;
// 		}
// 		quote = '\0';
// 	}
// 	if (quote)
// 		*input = ft_strjoin_and_free(*input,
// 				ft_strjoin_and_free(ft_strdup("\n"),
// 					// here_doc_word(ft_substr(*input, 0, i), quote, env)));
// 					//here_doc_word(*input, status, env)));
// 	return ;
// }

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
