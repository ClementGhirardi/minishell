/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:44:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 13:47:28 by cghirard         ###   ########.fr       */
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

int	run_builtin(char **args, char ***env, int status)
{
	if (!ft_strncmp(args[0], "echo", 4))
		return (ft_echo(args));
	if (!ft_strncmp(args[0], "pwd", 3))
		return (ft_pwd());
	if (!ft_strncmp(args[0], "cd", 2))
		return (ft_cd(args, env));
	if (!ft_strncmp(args[0], "export", 6))
		return (ft_export(args, env, status));
	if (!ft_strncmp(args[0], "unset", 5))
		return (ft_unset(args, env));
	if (!ft_strncmp(args[0], "env", 3))
		return (ft_env(env));
	if (!ft_strncmp(args[0], "exit", 4))
		return (ft_exit(args, status));
	return (1);
}
