/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/24 11:04:40 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	executor(t_ast *ast, char ***env);

volatile sig_atomic_t	g_signal = 0;

static int	run_cmd(char **args, char **env)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == 0)
	{
		path = get_path(args[0], env);
		if (!path)
			exit(127);
		execve(path, args, env);
		free(path);
		perror("execve");
		exit(126);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_cmd(t_ast *node, char ***env)
{
	if (is_builtin(node->args[0]))
		return (run_builtin(node->args, env));
	else
		return (run_cmd(node->args, *env));
}

int	execute_pipe(t_ast *node, char ***env)
{
	int		fd[2];
	pid_t	pid[2];
	int		status;

	if (pipe(fd) == -1)
		return (1);
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

int	execute_redir(t_ast *node, char ***env)
{
	int	fd;

	fd = -1;
	if (node->type == NODE_REDIR_IN)
	{
		fd = open(node->file, O_RDONLY);
		if (fd == -1)
			return (1);
		dup2(fd, STDIN_FILENO);
	}
	else
	{
		if (node->type == NODE_REDIR_OUT)
			fd = open(node->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (node->type == NODE_APPEND)
			fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (node->type == NODE_HEREDOC)
			fd = here_doc(node->file);
		if (fd == -1)
			return (1);
		dup2(fd, STDOUT_FILENO);
	}
	close(fd);
	return (executor(node->left, env));
}

int	executor(t_ast *ast, char ***env)
{
	if (!ast)
		return (1);
	if (ast->type == NODE_CMD)
		return (execute_cmd(ast, env));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, env));
	else if (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
		|| ast->type == NODE_APPEND || ast->type == NODE_HEREDOC)
		return (execute_redir(ast, env));
	return (1);
}
