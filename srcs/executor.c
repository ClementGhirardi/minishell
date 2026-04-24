/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/02 17:26:13 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	executor(t_ast *ast, char ***env)
{
	char	*tmp;

	if (ast && ast->type == NODE_CMD)
		return (expander(ast, *env), execute_cmd(ast, env));
	else if (ast && ast->type == NODE_PIPE)
		return (execute_pipe(ast, env));
	else if (ast && (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
			|| ast->type == NODE_APPEND || ast->type == NODE_HEREDOC))
	{
		if (!ast->file && ast->fd == -1)
			return (ft_putstr_fd("minishell: ", 2), 2);//ft_putendl_fd(
					//"syntax error near unexpected token `newline'", 2), 2);
		if (ast->file)
		{
			tmp = ft_strdup(ast->file);
			expander(ast, *env);
			if (!ast->file && ast->fd == -1)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(tmp, 2),
					ft_putendl_fd(": ambiguous redirect", 2), free(tmp), 2);
			free(tmp);
		}
		return (execute_redir(ast, env));
	}
	else
		return (execute_operator(ast, env));
}
