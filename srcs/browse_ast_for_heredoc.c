/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_ast_for_heredoc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:48:16 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/27 13:51:32 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	browse_ast_for_heredoc(t_ast *ast, t_data *data)
{
	if (!ast)
		return ;
	if (ast->type == NODE_HEREDOC)
	{
		ast->file = remove_limiters_quotes(ast->file);
		if (!ast->file)
			return ;
		ast->fd = here_doc(ast->file, data, ast);
		if (ast->fd == -1)
			return ;
	}
	browse_ast_for_heredoc(ast->left, data);
	browse_ast_for_heredoc(ast->right, data);
}
