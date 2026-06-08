/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:44:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 11:02:10 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 4) || !ft_strncmp(cmd, "cd", 2)
		|| !ft_strncmp(cmd, "pwd", 3) || !ft_strncmp(cmd, "export", 6)
		|| !ft_strncmp(cmd, "unset", 5) || !ft_strncmp(cmd, "env", 3)
		|| !ft_strncmp(cmd, "exit", 4));
}

// int	run_builtin(char **args, char ***env, int status)
// {
// 	if (!ft_strncmp(args[0], "pwd", 3))
// 		return (ft_pwd());
// 	if (!ft_strncmp(args[0], "cd", 2))
// 		return (ft_cd(args, env));
// 	if (!ft_strncmp(args[0], "export", 6))
// 		return (ft_export(args, env));
// 	if (!ft_strncmp(args[0], "unset", 5))
// 		return (ft_unset(args, env));
// 	if (!ft_strncmp(args[0], "env", 3))
// 		return (ft_env(env));
// 	if (!ft_strncmp(args[0], "exit", 4))
// 		return (ft_exit(env, status));
// 	if (!ft_strncmp(args[0], "echo", 4))
// 		return (ft_echo(args, env));
// 	return (1);
// }

int	run_builtin(t_ast *node, t_data *data, int fd_in, int fd_out)
{
	if (!ft_strcmp(node->args[0], "pwd"))
		return (ft_pwd(fd_out));
	if (!ft_strcmp(node->args[0], "cd"))
		return (ft_cd(node->args, &data->env, fd_out));
	if (!ft_strcmp(node->args[0], "export"))
		return (ft_export(node->args, &data->env, *data->status, fd_out));
	if (!ft_strcmp(node->args[0], "unset"))
		return (ft_unset(node->args, &data->env));
	if (!ft_strcmp(node->args[0], "env"))
		return (ft_env(&data->env, fd_out));
	if (!ft_strcmp(node->args[0], "exit"))
		return (ft_exit(data, node, fd_in, fd_out));
	if (!ft_strcmp(node->args[0], "echo"))
		return (ft_echo(node->args, fd_out));
	return (1);
}
