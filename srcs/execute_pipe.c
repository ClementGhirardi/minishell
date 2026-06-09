/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:48:43 by clement-ghi       #+#    #+#             */
/*   Updated: 2026/06/09 10:06:24 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_exit_in_pipes(t_data *data, t_ast *node, int fd_in, int fd_out)
{
	int	too_many;
	int	not_digit;

	too_many = 0;
	not_digit = 0;
	*data->status = check_exit_args(data, node, &too_many, &not_digit);
	if (too_many)
		return (ft_putendl_fd("minishell: exit: too many arguments", 2),
			ast_free(data->ast), data->ast = NULL, *data->status = 1, 1);
	if (not_digit)
		error_num(node->args[1], data->status);
	if (wait(NULL) != -1)
		return (free_and_close(data, fd_in, fd_out), *data->status);
	if (wait(NULL) != -1)
		return (free_and_close(data, fd_in, fd_out), *data->status);
	free_and_close(data, fd_in, fd_out);
	rl_clear_history();
	if (data && data->input)
		free(*data->input);
	exit(*data->status);
}

static int	execute_builtin_with_pipe(t_ast *node, t_data *data,
			int fd_in, int fd_out)
{
	pid_t	pid;
	int		code;

	if (!node || !node->args || !node->args[0])
		return (1);
	if (node->args[0][0] && err_exe_cmd(node->args[0], data->status, data->env))
		return (*data->status);
	pid = fork();
	if (pid == 0)
	{
		expander(node, *data->status, data->env);
		if (!ft_strcmp(node->args[0], "exit"))
			code = ft_exit_in_pipes(data, node, fd_in, fd_out);
		else
			code = run_builtin(node, data, fd_in, fd_out);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (fd_out != STDOUT_FILENO)
			close(fd_out);
		if (data->input)
			free(*data->input);
		return (free_array(data->env), ast_free(data->ast),
			exit(code), code);
	}
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
	if (data->input)
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
	if (data->input)
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
