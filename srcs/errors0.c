/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:27:59 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/04 18:20:11 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	error_creating_env(void)
{
	ft_putendl_fd("minishell: can't create new environment", 2);
	return (1);
}

static int	is_an_env_var(char *path, char **env)
{
	int	i;
	int	j;

	if (!path)
		return (0);
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		if (!ft_strcmp(&env[i][j + 1], path))
			return (1);
		i++;
	}
	return (0);
}

static int	error_exec_cmd_slash(char *arg, int *status, char **env)
{
	if (arg[0] == '/')
	{
		*status = 126;
		if (!existing_path(arg))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd(": No such file or directory", 2);
		}
		else if (is_an_env_var(arg, env))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd(": Is a directory", 2);
		}
		else
			return (0);
		return (1);
	}
	return (0);
}

int	err_exe_cmd(char *arg, int *status, char **env)
{
	DIR	*dir;

	if (error_exec_cmd_slash(arg, status, env))
		return (1);
	if (!ft_strncmp(arg, ".", ft_strlen(arg)))
		return (*status = 2,
			ft_putendl_fd("minishell: .: filename argument required", 2),
			ft_putendl_fd(".: usage: . filename [arguments]", 2), 1);
	if (!ft_strncmp(arg, "./", 2))
	{
		dir = opendir(&arg[2]);
		if (!dir)
			return (0);
		closedir(dir);
		*status = 126;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": Is a directory", 2);
		return (1);
	}
	return (0);
}

int	error_here_doc(int *fd, int nb_line, char *limiter, int status)
{
	if (status == 130)
		return (close(fd[1]), close(fd[0]), -1);
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(nb_line, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiter, 2);
	ft_putendl_fd("')", 2);
	return (close(fd[1]), fd[0]);
}
