/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 13:36:03 by clement-ghi       #+#    #+#             */
/*   Updated: 2026/06/05 12:17:51 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

int	run_builtin(char **args, t_data *data, int fd_in, int fd_out)
{
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd(fd_out));
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(args, &data->env, fd_out));
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(args, &data->env, *data->status, fd_out));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(args, &data->env));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env(&data->env, fd_out));
	if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(data, data->env, fd_in, fd_out));
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(args, fd_out));
	return (1);
}
