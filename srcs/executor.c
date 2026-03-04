/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/04 12:04:49 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute(t_ast *node, t_instrs *instrs)
{
	if (!node)
		return ;
	if (node->type == NODE_CMD)
		exec_cmd();
	else if (node->type == NODE_REDIR_IN
		|| node->type == NODE_REDIR_OUT
		|| node->type == NODE_APPEND
		|| node->type == NODE_HEREDOC)
		exec_redir();
	else if (node->type == NODE_PIPE)
	{
		executor(node->left, instrs);
		executor(node->right,instrs);
	}
}

static int	existing_path(char *cmd)
{
	if (!access(cmd, F_OK))
	{
		if (access(cmd, X_OK))
			perror(cmd);
		return (1);
	}
	return (0);
}

static char	**get_paths(char **envp)
{
	char	**paths;
	int		i;

	if (!envp[0])
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

static char	*full_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*tmp;
	char	*full;

	if (existing_path(cmd))
		return (ft_strdup(cmd));
	paths = get_paths(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!cmd)
			return (free(tmp), free_strs(paths), NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(full, X_OK))
			return (free_strs(paths), full);
		free(full);
		i++;
	}
	free_strs(paths);
	return (NULL);
}

void	executor(t_ast *node,char **envp)
{
	t_instrs	instrs;

	instrs.fd_in = 0;
	instrs.fd_out = 1;
	instrs.args = node->args;
	instrs.path = full_path(node->args, envp);
	execute(node, &instrs);
}
