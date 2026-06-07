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

#include "../includes/minishell.h"

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
	return (1);
}
