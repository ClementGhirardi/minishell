/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:48:59 by clement-ghi       #+#    #+#             */
/*   Updated: 2026/06/08 16:52:10 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	error_perm(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	if (str)
		ft_putstr_fd(str, 2);
	ft_putstr_fd(": Permission denied\n", 2);
}

static void	execute_cmd_child(int fd_in, int fd_out)
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
}

static void	sigquit_handler(int sig)
{
	(void)sig;
	ft_putendl_fd("Quit (core dumped)", 1);
}

int	execute_cmd(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	pid_t	pid;
	char	*path;

	if (!node || !node->args || !node->args[0] || (node->args[0][0]
		&& err_exe_cmd(node->args[0], data->status, data->env)))
		return (*data->status);
	if (is_builtin(node->args[0]))
		return (run_builtin(node, data, fd_in, fd_out));
	path = get_path(node->args[0], data->env);
	if (!path)
		return (error_command(node->args[0]), 127);
	else
	{
		signal(SIGQUIT, sigquit_handler);
		pid = fork();
		if (pid == 0)
		{
			execute_cmd_child(fd_in, fd_out);
			execve(path, node->args, data->env);
			return (free(path), error_perm(node->args[0]),
				free_array(data->env), ast_free(data->ast), exit(126), 126);
		}
	}
	return (waitpid(pid, data->status, 0), signal(SIGQUIT, SIG_IGN),
		free(path), get_status(*data->status));
}
