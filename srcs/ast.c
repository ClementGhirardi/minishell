/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:17:06 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/05 14:36:51 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*ast_new_cmd(char **args)
{
	t_ast	*node;

	node = malloc(1 * sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->args = args;
	node->file = NULL;
	node->fd = -1;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast	*ast_new_redir(t_token_type r_type, int *status, char **env,
		t_token *tokens)
{
	t_ast	*node;

	node = create_redir_node(r_type, status, env, tokens);
	if (!node)
		return (NULL);
	node->args = NULL;
	if (r_type == TOKEN_REDIR_IN)
		node->type = NODE_REDIR_IN;
	if (r_type == TOKEN_REDIR_OUT)
		node->type = NODE_REDIR_OUT;
	if (r_type == TOKEN_APPEND)
		node->type = NODE_APPEND;
	if (r_type == TOKEN_HEREDOC)
		node->type = NODE_HEREDOC;
	node->fd = -1;
	return (node->left = NULL, node->right = NULL, node->args = NULL, node);
}

t_ast	*ast_new_pipe(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->args = NULL;
	node->file = NULL;
	node->fd = -1;
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

void	ast_free(t_ast *ast)
{
	int		i;

	if (ast)
	{
		if (ast->args)
		{
			i = 0;
			while (ast->args[i])
			{
				free(ast->args[i]);
				i++;
			}
			free(ast->args);
		}
		if (ast->file)
			free(ast->file);
		ast_free(ast->left);
		ast_free(ast->right);
		free(ast);
	}
}
