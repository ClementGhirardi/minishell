/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/04 16:01:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	existing_path(char *cmd)
// {
// 	if (!access(cmd, F_OK))
// 	{
// 		if (access(cmd, X_OK))
// 			perror(cmd);
// 		return (1);
// 	}
// 	return (0);
// }

// static char	**get_paths(char **envp)
// {
// 	char	**paths;
// 	int		i;

// 	if (!envp[0])
// 		return (NULL);
// 	i = 0;
// 	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
// 		i++;
// 	if (!envp[i])
// 		return (NULL);
// 	paths = ft_split(envp[i] + 5, ':');
// 	return (paths);
// }

// void	free_strs(char **strs)
// {
// 	int	i;

// 	if (!strs)
// 		return ;
// 	i = 0;
// 	while (strs[i])
// 	{
// 		free(strs[i]);
// 		i++;
// 	}
// 	free(strs);
// }

// static char	*full_path(char *cmd, char **envp)
// {
// 	int		i;
// 	char	**paths;
// 	char	*tmp;
// 	char	*full;

// 	if (existing_path(cmd))
// 		return (ft_strdup(cmd));
// 	paths = get_paths(envp);
// 	if (!paths)
// 		return (NULL);
// 	i = 0;
// 	while (paths[i])
// 	{
// 		tmp = ft_strjoin(paths[i], "/");
// 		if (!cmd)
// 			return (free(tmp), free_strs(paths), NULL);
// 		full = ft_strjoin(tmp, cmd);
// 		free(tmp);
// 		if (!access(full, X_OK))
// 			return (free_strs(paths), full);
// 		free(full);
// 		i++;
// 	}
// 	free_strs(paths);
// 	return (NULL);
// }

void	exec_cmd(t_ast *node, int *fd_in, int *fd_out, char **envp)
{
	(void) node;
	(void) envp;
	ft_putstr_fd(node->args[0], *fd_out);
	ft_printf("(%d, %d)\n",*fd_in, *fd_out);
}

int	here_doc(char *limiter)
{
	char	*buffer;
	int		size;
	int		fd[2];

	pipe(fd);
	write(1, "> ", 15);
	size = ft_strlen(limiter);
	buffer = get_next_line(STDIN_FILENO);
	while (ft_strncmp(buffer, limiter, size) || buffer[size] != '\n')
	{
		ft_putstr_fd(buffer, fd[1]);
		write(1, "> ", 15);
		free(buffer);
		buffer = get_next_line(STDIN_FILENO);
	}
	free(buffer);
	close(fd[1]);
	return (fd[0]);
}

void	exec_redir(t_ast *node, int *fd_in, int *fd_out)
{
	if (node->type == NODE_REDIR_IN)
	{
		close(*fd_in);
		*fd_in = open(node->file, O_RDONLY);
	}
	else if (node->type == NODE_REDIR_OUT)
	{
		close(*fd_out);
		*fd_out = open(node->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	else if (node->type == NODE_APPEND)
	{
		close(*fd_out);
		*fd_out = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else if (node->type == NODE_HEREDOC)
	{
		close(*fd_in);
		*fd_in = here_doc(node->file);
	}
}

void	execute(t_ast *node, int *fd_in, int *fd_out, char **envp)
{
	if (!node)
		return ;
	if (node->type == NODE_CMD)
		exec_cmd(node, fd_in, fd_out, envp);
	else if (node->type == NODE_REDIR_IN
		|| node->type == NODE_REDIR_OUT
		|| node->type == NODE_APPEND
		|| node->type == NODE_HEREDOC)
	{
		exec_redir(node, fd_in, fd_out);
		execute(node->left, fd_in, fd_out, envp);
	}
	else if (node->type == NODE_PIPE)
	{
		execute(node->left, fd_in, fd_out, envp);
		execute(node->right, fd_in, fd_out, envp);
	}
}

void	executor(t_ast *node,char **envp)
{
	int	fd_in;
	int	fd_out;

	fd_in = open("infile", O_RDONLY);
	fd_out = open("outfile", O_WRONLY);;
	execute(node, &fd_in, &fd_out, envp);
	close(fd_in);
}
