/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/03 13:03:46 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell.h"

static int	execute_operator(t_ast *ast, t_data *data, int fd_in, int fd_out)
{
	static int	fail;

	if (!ast)
		return (1);
	*data->status = executor(ast->left, data, fd_in, fd_out);
	if (ast->left && ast->left->type == NODE_CMD)
		fail = *data->status;
	if (ast->type == NODE_AND)
	{
		if (!*data->status || !fail)
			*data->status = executor(ast->right, data, fd_in, fd_out);
	}
	else if (ast->type == NODE_OR)
	{
		if (*data->status || fail)
			*data->status = executor(ast->right, data, fd_in, fd_out);
	}
	if (ast->right && ast->right->type == NODE_CMD)
		fail = *data->status;
	return (*data->status);
}

int	executor(t_ast *ast, t_data *data, int fd_in, int fd_out)
{
	if (!ast)
		return (1);
	if (g_sig_status == 2)
		return (130);
	if (ast && ast->type == NODE_CMD)
		return (expander(ast, *data->status, data->env),
			execute_cmd(ast, data, fd_in, fd_out));
	else if (ast && ast->type == NODE_PIPE)
		return (execute_pipe(ast, data, fd_in, fd_out));
	else if (ast && (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
			|| ast->type == NODE_APPEND || ast->type == NODE_HEREDOC))
	{
		if (ast->file && ast->type != NODE_HEREDOC)
			expander(ast, *data->status, data->env);
		return (execute_redir(ast, data, fd_in, fd_out));
	}
	else if (ast && (ast->type == NODE_AND || ast->type == NODE_OR))
		return (execute_operator(ast, data, fd_in, fd_out));
	return (1);
}
