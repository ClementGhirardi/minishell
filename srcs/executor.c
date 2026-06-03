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

int	execute_operator(t_ast *ast, t_data *data, int fd_in, int fd_out)
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

int	execute_cmd(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	pid_t	pid;
	char	*path;

	if (!node || !node->args || !node->args[0])
		return (0);
	if (node->args[0][0] && err_exe_cmd(node->args[0], data->status, data->env))
		return (*data->status);
	if (is_builtin(node->args[0]))
		return (run_builtin(node->args, data, fd_in, fd_out));
	path = get_path(node->args[0], data->env);
	if (!path)
		return (error_command(node->args[0]), 127);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			execute_cmd_child(fd_in, fd_out);
			execve(path, node->args, data->env);
			return (free(path), error_perm(node->args[0]),
				free_array(data->env), ast_free(data->ast), exit(126), 126);
		}
	}
	return (waitpid(pid, data->status, 0),
		free(path), get_status(*data->status));
}

int	execute_pipe(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	int		fd[2];
	pid_t	pid[2];

	if (pipe(fd) == -1)
		return (1);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		close(fd[0]);
		if (fd_out != STDOUT_FILENO)
			close(fd_out);
		*data->status = handle_pipe_execution(data, node->left, fd_in, fd[1]);
		close_free_exit(data, fd[1], fd_in);
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		close(fd[1]);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		*data->status = handle_pipe_execution(data, node->right, fd[0], fd_out);
		close_free_exit(data, fd[0], fd_out);
	}
	return (close(fd[0]), close(fd[1]), waitpid(pid[0], data->status, 0),
		waitpid(pid[1], data->status, 0), get_status(*data->status));
}

static int	execute_redir(t_ast *node, t_data *data, int fd_in, int fd_out)
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
	if (node->type == NODE_HEREDOC)
		return (*data->status);
	return (close(fd), *data->status);
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
