/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_after.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:26:21 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 14:26:23 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_after_cbracket(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (tokens->type != TOKEN_WORD
		&& tokens->type != TOKEN_O_BRACK
		&& tokens->type != TOKEN_C_BRACK)
		return (0);
	return (1);
}

int	syntax_after_obracket(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type != TOKEN_WORD
		&& tokens->type != TOKEN_O_BRACK
		&& tokens->type != TOKEN_C_BRACK)
		return (0);
	return (1);
}

int	syntax_after_redir(t_token *tokens)
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

int	syntax_after_token(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (tokens->type == TOKEN_AND
		|| tokens->type == TOKEN_OR
		|| tokens->type == TOKEN_PIPE)
		return (0);
	return (1);
}
