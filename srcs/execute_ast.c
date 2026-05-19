/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbarth <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 11:35:34 by adbarth           #+#    #+#             */
/*   Updated: 2026/04/17 11:35:36 by adbarth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	executor(t_ast *ast, t_ast *root, int status, char ***env);

int	execute_cmd(t_ast *node, t_ast *root,  int status, char ***env)
{
	pid_t	pid;
	char	*path;

	if (!node || !node->args || !node->args[0])
		return (0);
	if (error_exec_cmd(node->args[0], &status, *env))
		return (status);
	if (is_builtin(node->args[0]))
		return (run_builtin(node, node->args, env, status));
	else
	{
		pid = fork();
		if (pid == 0)
		{
			path = get_path(node->args[0], *env);
			if (!path)
				return (error_command(node->args[0]), free_array(*env),
						ast_free(root), exit(127), 127);
			execve(path, node->args, *env);
			free(path);
			return (ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd(node->args[0], 2), ft_putendl_fd(
					": Permission denied", 2), free_array(*env), ast_free(root),
					 exit(126), 126);
		}
		return (waitpid(pid, &status, 0), get_status(status));
	}
}

int	execute_pipe(t_ast *node, t_ast *root, int status, char ***env)
{
	int		fd[2];
	pid_t	pid[2];

	if (pipe(fd) == -1)
		return (1);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		status = executor(node->left, root, status, env);
		ast_free(root);
		free_array(*env);
		exit(status);
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		status = executor(node->right, root, status, env);
		ast_free(root);
		free_array(*env);
		exit(status);
	}
	return (close(fd[0]), close(fd[1]), waitpid(pid[0], &status, 0),
		waitpid(pid[1], &status, 0), get_status(status));
}

static int	open_fd(t_ast *node, char *file)
{
	int			fd;

	fd = -1;
	// if (dollar_finder(node->file)
	// 	&& !expand_dollar_in_filename(ft_strdup(node->file), env))
	// {
	// 	ft_putstr_fd("minishell: ", 2);
	// 	ft_putstr_fd(node->file, 2);
	// 	ft_putendl_fd(": ambiguous redirect", 2);
	// 	return (fd);
	// }
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

int	execute_redir(t_ast *node, t_ast *root, int status, char ***env)
{
	int	fd;
	int	std[2];

	fd = -1;
	if (!node->file || !node->file[0])
		return (1);
	if (node->file && node->file[0])
		fd = open_fd(node, node->file);
	if (fd == -1 && node->file && node->file[0])
		return (error_open(node->file));
	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
	{
		std[0] = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		status = executor(node->left, root, status, env);
		dup2(std[0], STDIN_FILENO);
		close(std[0]);
	}
	else
	{
		std[1] = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		status = executor(node->left, root, status, env);
		dup2(std[1], STDOUT_FILENO);
		close(std[1]);
	}
	return (close(fd), status);
}

int	execute_operator(t_ast *ast, t_ast *root, int status, char ***env)
{
	static int	stop;

	if (!ast)
	{
		stop = 1;
		return (stop);
	}
	stop = executor(ast->left, root, status, env);
	status = stop;
	if (ast->type == NODE_AND)
	{
		if (!stop)
			stop = executor(ast->right, root, status, env);
	}
	else
	{
		if (stop)
			stop = executor(ast->right, root, status, env);
	}
	status = stop;
	return (stop);
}
