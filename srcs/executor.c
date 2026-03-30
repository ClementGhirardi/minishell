/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/30 11:16:33 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	executor(t_ast *ast, char ***env);

int	execute_cmd(t_ast *node, char ***env)
{
	pid_t	pid;
	int		status;
	char	*path;

	if (is_builtin(node->args[0]))
		return (run_builtin(node->args, env));
	else
	{
		pid = fork();
		if (pid == 0)
		{
			path = get_path(node->args[0], *env);
			if (!path)
				return (ft_printf("minishell: %s: command not found\n",
						node->args[0]), exit(127), 127);
			execve(path, node->args, *env);
			free(path);
			return (perror("execve"), exit(126), 126);
		}
		waitpid(pid, &status, 0);
		return (get_status(status));
	}
}

int	execute_pipe(t_ast *node, char ***env)
{
	int		fd[2];
	pid_t	pid[2];
	int		status;

	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		exit(executor(node->left, env));
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		exit(executor(node->right, env));
	}
	return (close(fd[0]), close(fd[1]),
		waitpid(pid[0], &status, 0), waitpid(pid[1], &status, 0),
		get_status(status));
}

static int	open_fd(t_node_type type, char *file)
{
	int	fd;

	fd = -1;
	if (type == NODE_REDIR_IN)
		fd = open(file, O_RDONLY);
	else if (type == NODE_HEREDOC)
		fd = here_doc(file);
	else if (type == NODE_REDIR_OUT)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == NODE_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	execute_redir(t_ast *node, char ***env)
{
	int	fd;
	int	status;
	int	std[2];

	fd = open_fd(node->type, node->file);
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
	status = executor(node->left, env);
	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
		dup2(std[0], STDIN_FILENO);
	else
		dup2(std[1], STDOUT_FILENO);
	return (status);
}

int	executor(t_ast *ast, char ***env)
{
	if (!ast)
	{
		return (1);
	}
	if (ast->type == NODE_CMD)
		return (execute_cmd(ast, env));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, env));
	else if (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
		|| ast->type == NODE_APPEND || ast->type == NODE_HEREDOC)
		return (execute_redir(ast, env));
	return (1);
}
