/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error0.c.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:22:50 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 14:22:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		if (is_an_env_var(arg, env))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd(": Is a directory", 2);
		}
		else if (!existing_path(arg))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd(": No such file or directory", 2);
		}
		else
			return (0);
		return (1);
	}
	return (0);
}

int	error_exec_cmd(char *arg, int *status, char **env)
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
	ft_putstr_fd("\nminishell: warning: here-document at line ", 2);
	ft_putnbr_fd(nb_line, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiter, 2);
	ft_putendl_fd("')", 2);
	return (close(fd[1]), fd[0]);
}

void	error_file(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	if (file)
		ft_putstr_fd(file, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
}
