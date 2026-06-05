/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:48:43 by clement-ghi       #+#    #+#             */
/*   Updated: 2026/06/05 12:43:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	execute_builtin_with_pipe(t_ast *node, t_data *data,
			int fd_in, int fd_out)
{
	pid_t	pid;
	int		code;

	if (!node || !node->args || !node->args[0])
		return (1);
	if (node->args[0][0]
		&& err_exe_cmd(node->args[0], data->status, data->env))
		return (*data->status);
	if (is_builtin(node->args[0]))
	{
		pid = fork();
		if (pid == 0)
		{
			expander(node, *data->status, data->env);
			code = run_builtin(node->args, data, fd_in, fd_out);
			if (fd_in != STDIN_FILENO)
				close(fd_in);
			if (fd_out != STDOUT_FILENO)
				close(fd_out);
			return (free_array(data->env), ast_free(data->ast),
				exit(code), code);
		}
	}
	else
		return (1);
	return (waitpid(pid, data->status, 0), get_status(*data->status));
}

static void	child_left(t_ast *node, t_data *data, int *prev_fd, int *fd)
{
	int	fd_in;
	int	fd_out;

	fd_in = prev_fd[0];
	fd_out = prev_fd[1];
	close(fd[0]);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	if (node->left && node->left->type == NODE_CMD
		&& node->left->args && is_builtin(node->left->args[0]))
		*data->status = execute_builtin_with_pipe(
				node->left, data, fd_in, fd[1]);
	else
		*data->status = executor(node->left, data, fd_in, fd[1]);
	close(fd[1]);
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	ast_free(data->ast);
	free_array(data->env);
	free(*data->input);
	exit(*data->status);
}

static void	child_right(t_ast *node, t_data *data, int *prev_fd, int *fd)
{
	int	fd_in;
	int	fd_out;

	fd_in = prev_fd[0];
	fd_out = prev_fd[1];
	close(fd[1]);
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (node->right && node->right->type == NODE_CMD
		&& node->right->args && is_builtin(node->right->args[0]))
		*data->status = execute_builtin_with_pipe(
				node->right, data, fd[0], fd_out);
	else
		*data->status = executor(node->right, data, fd[0], fd_out);
	close(fd[0]);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	ast_free(data->ast);
	free_array(data->env);
	free(*data->input);
	exit(*data->status);
}

int	execute_pipe(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	int		prev_fd[2];
	int		fd[2];
	pid_t	pid[2];

	prev_fd[0] = fd_in;
	prev_fd[1] = fd_out;
	if (pipe(fd) == -1)
		return (1);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		child_left(node, data, prev_fd, fd);
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		child_right(node, data, prev_fd, fd);
	}
	return (close(fd[1]), waitpid(pid[0], data->status, 0), close(fd[0]),
		waitpid(pid[1], data->status, 0), get_status(*data->status));
}
