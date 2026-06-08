/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 11:49:46 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*parse_pipeline(t_token **tokens, t_data *data)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*brack;
	t_token	*tmp;

	left = parse_instructions(tokens, data);
	while (tokens && *tokens && (*tokens)->type == TOKEN_PIPE
		&& g_sig_status != 4)
	{
		brack = NULL;
		if ((*tokens)->bracket)
		{
			tmp = (*tokens)->bracket;
			brack = parser(&tmp, data);
			left = ast_new_pipe_op(left, brack, TOKEN_PIPE);
			*tokens = (*tokens)->next;
		}
		else
		{
			*tokens = (*tokens)->next;
			right = parse_instructions(tokens, data);
			left = ast_new_pipe_op(left, right, TOKEN_PIPE);
		}
	}
	return (left);
}

t_ast	*parse_after_bracket(t_token **tokens, t_ast *left, t_ast *brack,
	t_data *data)
{
	t_ast			*instr;
	t_token_type	type;

	if (!tokens || !*tokens)
		return (NULL);
	type = (*tokens)->type;
	instr = parse_pipeline(tokens, data);
	if (!brack && (type == TOKEN_APPEND || type == TOKEN_HEREDOC
			|| type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT))
	{
		instr->left = left;
		return (instr);
	}
	left->right = instr;
	instr->left = brack;
	return (left);
}

t_ast	*parsing_loop(t_token **tokens, t_ast *left, t_ast **brack,
	t_data *data)
{
	t_ast			*right;
	t_token			*tmp;
	t_token_type	type;

	while ((*tokens && (*tokens)->type == TOKEN_AND)
		|| (*tokens && (*tokens)->type == TOKEN_OR))
	{
		*brack = NULL;
		type = (*tokens)->type;
		if ((*tokens)->bracket)
		{
			tmp = (*tokens)->bracket;
			*brack = parser(&tmp, data);
			left = ast_new_pipe_op(left, *brack, type);
			*tokens = (*tokens)->next;
		}
		else
		{
			*tokens = (*tokens)->next;
			right = parse_pipeline(tokens, data);
			left = ast_new_pipe_op(left, right, type);
		}
	}
	return (left);
}

t_ast	*parser(t_token **tokens, t_data *data)
{
	t_ast		*left;
	t_ast		*brack;

	if (!tokens || !*tokens)
		return (NULL);
	brack = NULL;
	left = parse_pipeline(tokens, data);
	while (*tokens && g_sig_status != 4)
	{
		left = parsing_loop(tokens, left, &brack, data);
		if (*tokens)
			left = parse_after_bracket(tokens, left, brack, data);
	}
	return (left);
}
