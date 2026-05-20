/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:17:06 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/02 14:32:49 by cghirard         ###   ########.fr       */
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

t_ast	*ast_new_redir(t_token_type r_type, int *status, char **env, t_data *data)
{
	t_ast		*node;

	node = create_redir_node(status, env, data);
	if (!node)
		return (NULL);
	if (r_type == TOKEN_REDIR_IN)
		node->type = NODE_REDIR_IN;
	if (r_type == TOKEN_REDIR_OUT)
		node->type = NODE_REDIR_OUT;
	if (r_type == TOKEN_APPEND)
		node->type = NODE_APPEND;
	if (r_type == TOKEN_HEREDOC)
	{
		node->type = NODE_HEREDOC;
		data->limiter = node->file;
		node->fd = here_doc(data, status, env);
		if (node->fd == -1)
			return (free(node->file), free(node), NULL);
	}
	else
	{
		node->fd = -1;
		node->args = NULL;
		node->left = NULL;
		node->right = NULL;
	}
	// node->args = NULL;
	// node->file = file;
	// node->left = NULL;
	// node->right = NULL;
	return (node);
	// return (node->left = NULL, node->right = NULL, node);
}

t_ast	*ast_new_pipe_op(t_ast *left, t_ast *right, t_token_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	if (type == TOKEN_AND)
		node->type = NODE_AND;
	else if (type == TOKEN_OR)
		node->type = NODE_OR;
	else
		node->type = NODE_PIPE;
	node->args = NULL; //iniialiser
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
