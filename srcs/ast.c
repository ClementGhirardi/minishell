/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:17:06 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/25 20:58:57 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*ast_new_node(t_node_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->file = NULL;
	node->quotes = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast	*ast_new_cmd(char **args, t_quote *quotes)
{
	t_ast	*node;

	node = ast_new_node(NODE_CMD);
	node->args = args;
	node->quotes = quotes;
	return (node);
}

t_ast	*ast_new_redir(t_node_type type, char *file,
	t_token_type token_type, t_ast *left)
{
	t_ast	*node;

	node = ast_new_node(type);
	node->file = file;
	node->quotes = malloc(1 * sizeof(t_quote));
	node->quotes[0] = token_type - TOKEN_WORD;
	node->left = left;
	return (node);
}

t_ast	*ast_new_pipe(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ast_new_node(NODE_PIPE);
	node->left = left;
	node->right = right;
	return (node);
}

t_node_type	token_to_node(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (NODE_REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (NODE_REDIR_OUT);
	if (type == TOKEN_APPEND)
		return (NODE_APPEND);
	if (type == TOKEN_HEREDOC)
		return (NODE_HEREDOC);
	return (NODE_CMD);
}
