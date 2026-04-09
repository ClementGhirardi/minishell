/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/02 17:26:13 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	executor(t_ast *ast, int status, char ***env);

static int	execute_cmd(t_ast *node, int status, char ***env)
{
	pid_t	pid;
	char	*path;

	if (!node->args)
		return (expander(node->left, status, env), execute_cmd(node->left, status, env));
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

static int	execute_pipe(t_ast *node, int status, char ***env)
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

static int	execute_redir(t_ast *node, int status, char ***env)
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
	}
	else
	{
		std[1] = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
	}
	status = executor(node->left, status, env);
	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
		dup2(std[0], STDIN_FILENO);
	else
		dup2(std[1], STDOUT_FILENO);
	return (status);
}

static int	execute_operator(t_ast *ast, int status, char ***env)
{
	static int	stop;

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

int	executor(t_ast *ast, int status, char ***env)
{
	char	*tmp;

	if (!ast)
		return (1);
	if (ast->type == NODE_CMD)
		return (expander(ast, status, env), execute_cmd(ast, status, env));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, status, env));
	else if (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
		|| ast->type == NODE_APPEND || ast->type == NODE_HEREDOC)
	{
		if (!ast->file && ast->fd == -1)
			return (ft_putstr_fd("minishell: ", 2), ft_putendl_fd(
					"syntax error near unexpected token `newline'", 2), 2);
		if (ast->file)
		{
			tmp = ft_strdup(ast->file);
			expander(ast, status, env);
			if (!ast->file && ast->fd == -1)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(tmp, 2),
					ft_putendl_fd(": ambiguous redirect", 2), free(tmp), 2);
		}
		return (execute_redir(ast, status, env));
	}
	else
		return (execute_operator(ast, status, env));
	return (1);
}
