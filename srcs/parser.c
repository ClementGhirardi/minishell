/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/05 16:04:49 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_ast	*parser_end_with_pipe(t_data *data)
{
	if (data->end_with_pipe)
	{
		free_token(data->tokens);
		browse_ast_for_heredoc(data->ast, data);
		if (data->ast && g_sig_status == 4)
			return (ast_free(data->ast), NULL);
		here_doc_word('\n', data);
		data->tokens = lexer(data->input, data);
		if (!data->tokens)
			return (ast_free(data->ast), NULL);
		return (parser(data->tokens, data));
	}
	return (ast_free(data->ast), NULL);
}

static int	parse_begin(t_token **tokens, t_data *data)
{
	t_ast	*left;
	t_ast	*new_ast;

	left = parse_instructions(tokens, data->status, data->env);
	if (!left)
		return (1);
	if (data->ast)
	{
		new_ast = ast_new_pipe(data->ast, left);
		if (!new_ast)
			return (free_token(data->tokens), ast_free(data->ast), 1);
		data->ast = new_ast;
	}
	else
		data->ast = left;
	return (0);
}

t_ast	*parser(t_token *tokens, t_data *data)
{
	t_ast	*right;
	t_ast	*new_ast;

	if (parse_begin(&tokens, data))
		return (NULL);
	while (tokens && tokens->type == TOKEN_PIPE)
	{
		tokens = tokens->next;
		right = parse_instructions(&tokens, data->status, data->env);
		if (data->ast && !right)
			return (parser_end_with_pipe(data));
		new_ast = ast_new_pipe(data->ast, right);
		if (!new_ast)
			return (free_token(data->tokens),
				ast_free(data->ast), ast_free(right), NULL);
		data->ast = new_ast;
	}
	free_token(data->tokens);
	browse_ast_for_heredoc(data->ast, data);
	if (data->ast && g_sig_status == 4)
		return (ast_free(data->ast), NULL);
	return (data->ast);
}
