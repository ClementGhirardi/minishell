/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_syntax_analyzer.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:41:38 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/03 12:22:50 by clement-ghi      ###   ########.fr       */
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
	if (tokens->type == TOKEN_PIPE)
		return (0);
	return (1);
}

static int	dispatch(t_token *tokens)
{
	if (tokens->type != TOKEN_WORD
		&& !syntax_after_token(tokens->next))
		return (0);
	if ((tokens->type == TOKEN_REDIR_IN
			|| tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_HEREDOC
			|| tokens->type == TOKEN_APPEND)
		&& !syntax_after_redir(tokens->next))
		return (0);
	return (1);
}

static int	syntax_first_token(t_token *tokens, int *status)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE)
	{
		error_syntax(tokens->value, status);
		return (0);
	}
	return (1);
}

int	syntax_analyzer(t_token *tokens, int *status)
{
	if (!syntax_first_token(tokens, status))
		return (1);
	while (tokens)
	{
		if (!dispatch(tokens))
		{
			if (tokens->next)
				error_syntax(tokens->next->value, status);
			else
				error_syntax("newline", status);
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}
