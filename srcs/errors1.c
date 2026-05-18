/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:20:08 by clement-ghi       #+#    #+#             */
/*   Updated: 2026/05/18 11:19:08 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_error_point(char *arg, int *status)
{
	if (!ft_strncmp(arg, ".", ft_strlen(arg)))
		return (*status = 2,
			ft_putendl_fd("minishell: .: filename argument required", 2),
			ft_putendl_fd(".: usage: . filename [arguments]", 2), 1);
	return (0);
}

// static int	check_error_slash(char *arg, int *status)
// {
// 	if (arg[0] == '/')
// 	{
// 		*status = 126;
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(arg, 2);
// 		if (!access(arg, F_OK))
// 			ft_putendl_fd(": Is a directory", 2);
// 		else
// 			ft_putendl_fd(": No such file or directory", 2);
// 		return (1);
// 	}
// 	return (0);
// }

static int	check_error_point_and_slash(char *arg, int *status)
{
	DIR		*dir;

	if (!ft_strncmp(arg, "./", 2) || !ft_strncmp(arg, "/", 1))
	{
		if (access(arg, F_OK))
			return (*status = 126, ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd(arg, 2),
				ft_putendl_fd(": No such file or directory", 2), 1);
		dir = opendir(arg);
		if (dir)
			return (*status = 126, closedir(dir),
				ft_putstr_fd("minishell: ", 2), ft_putstr_fd(arg, 2),
				ft_putendl_fd(": Is a directory", 2), 1);
		if (access(arg, X_OK))
			return (*status = 126, ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd(arg, 2),
				ft_putendl_fd(": Permission denied", 2), 1);
	}
	return (0);
}

int	error_exec_cmd(char *arg, int *status)
{
	if (check_error_point(arg, status))
		return (1);
	if (check_error_point_and_slash(arg, status))
		return (1);
	return (0);
}
