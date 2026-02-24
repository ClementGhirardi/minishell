/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:17:06 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/24 00:21:30 by cghirard         ###   ########.fr       */
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
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast	*ast_new_cmd(char **args)
{
	t_ast	*node;

	node = ast_new_node(NODE_CMD);
	node->args = args;
	return (node);
}

t_ast	*ast_new_redir(t_node_type type, char *file, t_ast *left)
{
	t_ast	*node;

	node = ast_new_node(type);
	node->file = ft_strdup(file);
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
