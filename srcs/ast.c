/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:17:06 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/26 13:05:08 by cghirard         ###   ########.fr       */
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
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->args = args;
	node->quotes = quotes;
	return (node);
}

t_ast	*ast_new_redir(t_token_type redir_type, char *file,
	t_token_type word_type)
{
	t_ast		*node;

	node = ast_new_node();
	if (!node)
		return (NULL);
	if (redir_type == TOKEN_REDIR_IN)
		node->type = NODE_REDIR_IN;
	else if (redir_type == TOKEN_REDIR_OUT)
		node->type = NODE_REDIR_OUT;
	else if (redir_type == TOKEN_APPEND)
		node->type = NODE_APPEND;
	else if (redir_type == TOKEN_HEREDOC)
		node->type = NODE_HEREDOC;
	else
		node->type = NODE_CMD;
	node->file = file;
	node->quotes = malloc(1 * sizeof(t_quote));
	node->quotes[0] = word_type - TOKEN_WORD;
	return (node);
}

t_ast	*ast_new_pipe(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ast_new_node();
	if (!node)
		return (NULL);
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
