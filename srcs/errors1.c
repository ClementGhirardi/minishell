/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:20:08 by clement-ghi       #+#    #+#             */
/*   Updated: 2026/05/12 14:04:00 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	error_exec_cmd_slash(char *arg, int *status, char **env)
{
	if (arg[0] == '/')
	{
		*status = 126;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		if (get_path(arg, env))
			ft_putendl_fd(": Is a directory", 2);
		else
			ft_putendl_fd(": No such file or directory", 2);
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
