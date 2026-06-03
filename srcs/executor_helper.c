/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/01 16:53:22 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_cmd_child(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}

void	close_free_exit(t_data *data, int fd1, int fd2)
{
	if (data)
	{
		ast_free(data->ast);
		free_array(data->env);
		if (data->input)
			free(*data->input);
	}
	if (fd1 != -1 && fd1 != STDIN_FILENO && fd1 != STDOUT_FILENO)
		close(fd1);
	if (fd2 != -1 && fd2 != STDIN_FILENO && fd2 != STDOUT_FILENO)
		close(fd2);
	exit(*data->status);
}

int	execute_builtin_with_pipe(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	pid_t	pid;
	int		code;

	if (!node || !node->args || !node->args[0])
		return (0);
	if (node->args[0][0] && err_exe_cmd(node->args[0], data->status, data->env))
		return (*data->status);
	if (is_builtin(node->args[0]))
	{
		pid = fork();
		if (pid == 0)
		{
			code = run_builtin(node->args, data, fd_in, fd_out);
			if (fd_in != STDIN_FILENO && fd_in != -1)
				close(fd_in);
			if (fd_out != STDOUT_FILENO && fd_in != -1)
				close(fd_out);
			return (free_array(data->env), ast_free(data->ast),
				exit(code), code);
		}
	}
	else
		return (1);
	return (waitpid(pid, data->status, 0), get_status(*data->status));
}

int	handle_pipe_execution(t_data *data, t_ast *node, int infd, int outfd)
{
	if (!node)
		return (1);
	if (node && node->type == NODE_CMD
		&& node->args && is_builtin(node->args[0]))
		return (execute_builtin_with_pipe(node, data, infd, outfd));
	else
		return (executor(node, data, infd, outfd));
}

int	open_fd(t_ast *node, char *file)
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
