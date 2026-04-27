/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/02 15:44:32 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_ast	*parse_instructions(t_token **tokens, t_infos *infos)
{
	t_ast	*instr;
	t_ast	*cmd;
	t_token	*tmp;

	if (!tokens || !(*tokens) || (*tokens)->type == TOKEN_PIPE
		|| (*tokens)->type == TOKEN_OR || (*tokens)->type == TOKEN_AND)
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD && (*tokens)->bracket)
	{
		tmp = (*tokens)->bracket;
		*tokens = (*tokens)->next;
		instr = parse(&tmp, infos->env, infos->input);
	}
	else if ((*tokens)->type == TOKEN_WORD)
	{
		cmd = parse_command(tokens);
		instr = parse_instructions(tokens, infos);
		ast_add_end(&instr, cmd);
	}
	else
	{
		instr = parse_redirection(tokens, infos);
		instr->left = parse_instructions(tokens, infos);
	}
	return (instr);
}

t_ast	*parse_pipeline(t_token **tokens, t_infos *infos)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*brack;
	t_token	*tmp;

	left = parse_instructions(tokens, infos);
	while (tokens && *tokens && (*tokens)->type == TOKEN_PIPE)
	{
		brack = NULL;
		if ((*tokens)->bracket)
		{
			tmp = (*tokens)->bracket;
			brack = parse(&tmp, infos->env, infos->input);
			left = ast_new_pipe_op(left, brack, TOKEN_PIPE);
			*tokens = (*tokens)->next;
		}
		else
		{
			*tokens = (*tokens)->next;
			right = parse_instructions(tokens, infos);
			left = ast_new_pipe_op(left, right, TOKEN_PIPE);
		}
	}
	return (left);
}

t_ast	*parse_after_bracket(t_token **tokens, t_ast *left, t_ast *brack,
	t_infos *infos)
{
	t_ast			*instr;
	t_token_type	type;

	if (!tokens || !*tokens)
		return (NULL);
	type = (*tokens)->type;
	instr = parse_pipeline(tokens, infos);
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
	t_infos *infos)
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
			*brack = parse(&tmp, infos->env, infos->input);
			left = ast_new_pipe_op(left, *brack, type);
			*tokens = (*tokens)->next;
		}
		else
		{
			*tokens = (*tokens)->next;
			right = parse_pipeline(tokens, infos);
			left = ast_new_pipe_op(left, right, type);
		}
	}
	return (left);
}

t_ast	*parse(t_token **tokens, char **env,
	char **input)
{
	//t_token		*tmp;
	t_ast		*left;
	t_ast		*brack;
	t_infos		infos;

	if (!tokens || !*tokens)
		return (NULL);
	infos.env = env;
	infos.input = input;
	brack = NULL;
	//tmp = *tokens;
	left = parse_pipeline(tokens, &infos);
	while (*tokens)
	{
		left = parsing_loop(tokens, left, &brack, &infos);
		if (*tokens)
			left = parse_after_bracket(tokens, left, brack, &infos);
	}
	//free_token(tmp);
	return (left);
}
