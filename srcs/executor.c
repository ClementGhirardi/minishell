/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/23 11:18:23 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	executor(t_ast *ast, char ***env);

volatile sig_atomic_t	g_signal = 0;

int	is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 4) || !ft_strncmp(cmd, "cd", 2)
		|| !ft_strncmp(cmd, "pwd", 3) || !ft_strncmp(cmd, "export", 6)
		|| !ft_strncmp(cmd, "unset", 5) || !ft_strncmp(cmd, "env", 3)
		|| !ft_strncmp(cmd, "exit", 4));
}

int	run_builtin(char **args, char ***env)
{
	(void) args;
	(void) env;
	return (1);
}

static int	existing_path(char *cmd)
{
	if (!access(cmd, F_OK))
	{
		if (access(cmd, X_OK))
			perror(cmd);
		return (1);
	}
	return (0);
}

static char	**get_paths(char **envp)
{
	char	**paths;
	int		i;

	if (!envp[0])
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static char	*full_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*tmp;
	char	*full;

	if (existing_path(cmd))
		return (ft_strdup(cmd));
	paths = get_paths(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!cmd)
			return (free(tmp), free_strs(paths), NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(full, X_OK))
			return (free_strs(paths), full);
		free(full);
		i++;
	}
	free_strs(paths);
	return (NULL);
}

int	run_cmd(char **args, char **env)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == 0)
	{
		path = full_path(args[0], env);
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
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(fd) == -1)
		return (1);
	pid_left = fork();
	if (pid_left == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		exit(executor(node->left, env));
	}
	pid_right = fork();
	if (pid_right == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		exit(executor(node->right, env));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	return (WEXITSTATUS(status));
}

int	here_doc(char *limiter)
{
	char	*buffer;
	int		size;
	int		fd[2];

	pipe(fd);
	write(1, "> ", 2);
	size = ft_strlen(limiter);
	buffer = get_next_line(STDIN_FILENO);
	while (ft_strncmp(buffer, limiter, size) || buffer[size] != '\n')
	{
		ft_putstr_fd(buffer, fd[1]);
		write(1, "> ", 2);
		free(buffer);
		buffer = get_next_line(STDIN_FILENO);
	}
	free(buffer);
	close(fd[1]);
	return (fd[0]);
}

int	execute_redir(t_ast *node, char ***env)
{
	int	fd;

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
