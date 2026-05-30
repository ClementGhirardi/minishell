/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/12 15:08:55 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_builtin_with_pipe(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	pid_t	pid;
	int 	code;

	if (!node || !node->args || !node->args[0])
		return (0);
	if (node->args[0][0] && error_exec_cmd(node->args[0], data->status, data->env))
		return (*data->status);
	if (is_builtin(node->args[0]))
	{
		pid = fork();
		if (pid == 0)
		{
			code = run_builtin(node->args, data, fd_in, fd_out);
			if (fd_in != STDIN_FILENO)
				close(fd_in);
			if (fd_out != STDOUT_FILENO)
				close(fd_out);
			return (free_array(data->env), ast_free(data->ast), exit(code), code);
		}
	}
	else
		return (1);
	return (waitpid(pid, data->status, 0), get_status(*data->status));
}

int	execute_cmd(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	pid_t	pid;
	char	*path;

	if (!node || !node->args || !node->args[0])
		return (0);
	if (node->args[0][0] && error_exec_cmd(node->args[0], data->status, data->env))
		return (*data->status);
	else if (is_builtin(node->args[0]))
		return (run_builtin(node->args, data, fd_in, fd_out));
	else
	{
		pid = fork();
		if (pid == 0)
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
			path = get_path(node->args[0], data->env);
			if (!path)
				return (error_command(node->args[0]), free_array(data->env),
					ast_free(data->ast), exit(127), 127);
			execve(path, node->args, data->env);
			free(path);
			return (ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd(node->args[0], 2), ft_putendl_fd(
					": Permission denied", 2), free_array(data->env), ast_free(data->ast),
				exit(126), 126);
		}
	}
	return (waitpid(pid, data->status, 0), get_status(*data->status));
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
		if (node->left && node->left->type == NODE_CMD
			&& node->left->args && is_builtin(node->left->args[0]))
			*data->status = execute_builtin_with_pipe(node->left, data, fd_in, fd[1]);
		else
			*data->status = executor(node->left, data, fd_in, fd[1]);
		close(fd[1]);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		ast_free(data->ast);
		free_array(data->env);
		exit(*data->status);
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		close(fd[1]);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (node->right && node->right->type == NODE_CMD
			&& node->right->args && is_builtin(node->right->args[0]))
			*data->status = execute_builtin_with_pipe(node->right, data, fd[0], fd_out);
		else
			*data->status = executor(node->right, data, fd[0], fd_out);
		close(fd[0]);
		if (fd_out != STDOUT_FILENO)
			close(fd_out);
		ast_free(data->ast);
		free_array(data->env);
		exit(*data->status);
	}
	return (close(fd[0]), close(fd[1]), waitpid(pid[0], data->status, 0),
		waitpid(pid[1], data->status, 0), get_status(*data->status));
}

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
	return (close(fd), *data->status);
}

int	executor(t_ast *ast, t_data *data, int fd_in, int fd_out)
{
	if (!ast)
		return (1);
	if (g_sig_status == 2)
		return (130);
	if (ast && ast->type == NODE_CMD)
		return (expander(ast, *data->status, data->env), execute_cmd(ast, data, fd_in, fd_out));
	else if (ast && ast->type == NODE_PIPE)
		return (execute_pipe(ast, data, fd_in, fd_out));
	else if (ast && (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
			|| ast->type == NODE_APPEND || ast->type == NODE_HEREDOC))
	{
		if (ast->file && ast->type != NODE_HEREDOC)
		{
			expander(ast, *data->status, data->env);
		}
		// if (ast->file)
		// {
		// 	expander(ast, status, *env);
		// 	// if (!ast->file)
		// 	// 	return (error_file())
		// 	if (!ast->file) // && ast->fd == -1)
		// 		return (ft_putstr_fd("minishell: ", 2),
		// 			ft_putstr_fd(ast->file, 2),
		// 			ft_putendl_fd(": ambiguous redirect", 2), 2);
		// }
		return (execute_redir(ast, data, fd_in, fd_out));
	}
	return (1);
}
