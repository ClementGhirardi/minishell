/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_bracket.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbarth <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 11:34:47 by adbarth           #+#    #+#             */
/*   Updated: 2026/04/17 11:34:51 by adbarth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	syntax_after_redir(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_REDIR_IN
		|| tokens->type == TOKEN_REDIR_OUT
		|| tokens->type == TOKEN_HEREDOC
		|| tokens->type == TOKEN_APPEND)
		return (0);
	return (1);
}

static int	syntax_after_token(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (tokens->type == TOKEN_AND
		|| tokens->type == TOKEN_OR
		|| tokens->type == TOKEN_PIPE)
		return (0);
	return (1);
}

static int	dispatch(t_token *tokens)
{
	if (tokens->type != TOKEN_WORD
		&& tokens->type != TOKEN_O_BRACK
		&& tokens->type != TOKEN_C_BRACK
		&& !syntax_after_token(tokens->next))
		return (0);
	if ((tokens->type == TOKEN_REDIR_IN
			|| tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_HEREDOC
			|| tokens->type == TOKEN_APPEND)
		&& !syntax_after_redir(tokens->next))
		return (0);
	if (tokens->type == TOKEN_O_BRACK)
	{
		if (tokens->next && tokens->next->type == TOKEN_C_BRACK)
			return (0);
		return (syntax_after_obracket(tokens->next));
	}
	if (tokens->type == TOKEN_C_BRACK)
		return ((tokens->next && tokens->next->type != TOKEN_O_BRACK)
			|| !tokens->next);
	return (1);
}

static int	syntax_first_token(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_AND
		|| tokens->type == TOKEN_OR
		|| tokens->type == TOKEN_PIPE
		|| tokens->type == TOKEN_C_BRACK)
	{
		syntax_error(tokens->value);
		return (0);
	}
	return (1);
}

int	syntax_analyzer(t_token *tokens)
{
	if (!syntax_first_token(tokens) || !check_brackets(tokens))
		return (0);
	while (tokens)
	{
		if (!dispatch(tokens))
		{
			if (tokens->next)
				syntax_error(tokens->next->value);
			else
				syntax_error("newline");
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}
