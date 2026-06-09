/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:48:12 by clement-ghi       #+#    #+#             */
/*   Updated: 2026/06/03 12:48:50 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	open_fd(t_ast *node, char *file)
{
	int			fd;

	fd = -1;
	if (node->type == NODE_REDIR_IN)
		fd = open(file, O_RDONLY);
	else if (node->type == NODE_HEREDOC)
		fd = node->fd;
	else if (node->type == NODE_REDIR_OUT)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (node->type == NODE_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	execute_redir(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	int	fd;

	fd = -1;
	if (!node->file || !node->file[0])
		return (1);
	if (node->file && node->file[0])
		fd = open_fd(node, node->file);
	if (fd == -1 && node->file && node->file[0])
		return (error_open(node->file));
	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
	{
		*data->status = executor(node->left, data, fd, fd_out);
	}
	else
	{
		*data->status = executor(node->left, data, fd_in, fd);
	}
	if (data && data->ast && node && node->type == NODE_HEREDOC)
		return (*data->status);
	return (close(fd), *data->status);
}
