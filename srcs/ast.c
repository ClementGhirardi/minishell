/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:17:06 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/24 16:02:33 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*ast_new_node(void)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = 0;
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

	node = ast_new_node();
	node->type = NODE_CMD;
	node->args = args;
	node->quotes = quotes;
	return (node);
}

t_ast	*ast_new_redir(t_node_type type, char *file,
	t_token_type token_type)
{
	t_ast	*node;

	node = ast_new_node();
	node->type = type;
	node->file = file;
	node->quotes = malloc(1 * sizeof(t_quote));
	node->quotes[0] = token_type - TOKEN_WORD;
	return (node);
}

t_ast	*ast_new_pipe(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ast_new_node();
	node->type = NODE_PIPE;
	node->left = left;
	node->right = right;
	return (node);
}

void	ast_add_end(t_ast **ast, t_ast *new)
{
	t_ast	*current;

	if (!ast || !new)
		return ;
	if (!(*ast))
	{
		*ast = new;
		return ;
	}
	current = *ast;
	while (current)
	{
		if (current->left == NULL)
		{
			current->left = new;
			return ;
		}
		current = current->left;
	}
	return ;
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
