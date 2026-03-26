/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/24 20:59:28 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*parse_command(t_token **tokens, t_ast *instr)
{
	char	**args;
	t_quote	*quotes;
	int		count;
	t_token	*tmp;
	int		i;

	count = 0;
	tmp = *tokens;
	while (tmp && (tmp->type == TOKEN_WORD
			|| tmp->type == TOKEN_WORD_SQUOTE
			|| tmp->type == TOKEN_WORD_DQUOTE))
	{
		count++;
		tmp = tmp->next;
	}
	if (count == 0)
		return (NULL);
	args = malloc((count + 1) * sizeof(char *));
	quotes = malloc((count) * sizeof(t_quote));
	i = 0;
	while (*tokens && ((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_WORD_SQUOTE
			|| (*tokens)->type == TOKEN_WORD_DQUOTE))
	{
		args[i] = ft_strdup((*tokens)->value);
		quotes[i++] = (*tokens)->type - TOKEN_WORD;
		*tokens = (*tokens)->next;
	}
	args[i] = NULL;
	instr = ast_new_cmd(args, quotes);
	return (instr);
}

t_ast	*parse_instructions(t_token **tokens);

t_ast	*parse_redirection(t_token **tokens, t_ast *instr)
{
	t_token		*tmp;
	t_node_type	type;

	tmp = *tokens;
	*tokens = (*tokens)->next;
	if (!*tokens || !((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_WORD_SQUOTE
			|| (*tokens)->type == TOKEN_WORD_DQUOTE))
		return (NULL);
	type = token_to_node(tmp->type);
	instr = ast_new_redir(type, (*tokens)->value, (*tokens)->type);
	*tokens = (*tokens)->next;
	return (instr);
}

t_ast	*parse_instructions(t_token **tokens)
{
	t_ast	*instr;
	t_ast	*cmd;

	instr = NULL;
	cmd = NULL;
	if (!(*tokens) || (*tokens)->type == TOKEN_PIPE)
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD
		|| (*tokens)->type == TOKEN_WORD_SQUOTE
		|| (*tokens)->type == TOKEN_WORD_DQUOTE)
	{
		cmd = parse_command(tokens, cmd);
		instr = parse_instructions(tokens);
		ast_add_end(&instr, cmd);
	}
	else
	{
		instr = parse_redirection(tokens, instr);
		instr->left = parse_instructions(tokens);
	}
	return (instr);
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_instructions(tokens);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_instructions(tokens);
		left = ast_new_pipe(left, right);
	}
	return (left);
}

t_ast	*parse(t_token *tokens)
{
	return (parse_pipeline(&tokens));
}
