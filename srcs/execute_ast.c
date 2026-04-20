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

int	executor(t_ast *ast, int status, char ***env);

int	execute_cmd(t_ast *node, int status, char ***env)
{
	pid_t	pid;
	char	*path;

	// if (!node->args)
	// 	return (expander(node->left, status, env), execute_cmd(node->left, status, env));
	if (is_builtin(node->args[0]))
		return (run_builtin(node->args, env, status));
	else
	{
		pid = fork();
		if (pid == 0)
		{
			path = get_path(node->args[0], *env);
			if (!path)
				return (write(2, "minishell: ", 12),
					write(2, node->args[0], ft_strlen(node->args[0])),
					write(2, ": command not found\n", 21),
					exit(127), 127);
			execve(path, node->args, *env);
			free(path);
			return (perror("execve"), exit(126), 126);
		}
		waitpid(pid, &status, 0);
		return (get_status(status));
	}
}

int	execute_pipe(t_ast *node, int status, char ***env)
{
	int		fd[2];
	pid_t	pid[2];

	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		exit(executor(node->left, status, env));
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		exit(executor(node->right, status, env));
	}
	return (close(fd[0]), close(fd[1]),
		waitpid(pid[0], &status, 0), waitpid(pid[1], &status, 0),
		get_status(status));
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

// int	execute_redir(t_ast *node, int status, char ***env)
// {
// 	int	fd;
// 	int	std[2];

// 	fd = open_fd(node, node->file);
// 	if (fd == -1)
// 		return (perror("open"), 1);
// 	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
// 	{
// 		std[0] = dup(STDIN_FILENO);
// 		dup2(fd, STDIN_FILENO);
// 	}
// 	else
// 	{
// 		std[1] = dup(STDOUT_FILENO);
// 		dup2(fd, STDOUT_FILENO);
// 	}
// 	status = executor(node->left, status, env);
// 	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
// 	{
// 		dup2(std[0], STDIN_FILENO);
// 		//close(std[0]);
// 	}
// 	else
// 	{
// 		dup2(std[1], STDOUT_FILENO);
// 		//close(std[1]);
// 	}
// 	return (status);
// }

int	execute_redir(t_ast *node, int status, char ***env)
{
	int	fd;
	int	std[2];

	fd = open_fd(node, node->file);
	if (fd == -1)
		return (perror("open"), 1);
	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
	{
		std[0] = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		status = executor(node->left, status, env);
		dup2(std[0], STDIN_FILENO);
		close(std[0]);
	}
	else
	{
		std[1] = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		status = executor(node->left, status, env);
		dup2(std[1], STDOUT_FILENO);
		close(std[1]);
	}
	close(fd);
	return (status);
}

int	execute_operator(t_ast *ast, int status, char ***env)
{
	static int	stop;

	if (!ast)
	{
		stop = 1;
		return (stop);
	}
	stop = executor(ast->left, status, env);
	if (ast->type == NODE_AND)
	{
		if (!stop)
			stop = executor(ast->right, status, env);
	}
	else
	{
		if (stop)
			stop = executor(ast->right, status, env);
	}
	return (stop);
}
