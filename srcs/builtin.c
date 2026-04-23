/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:44:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 14:43:08 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", ft_strlen(cmd))
		|| !ft_strncmp(cmd, "cd", ft_strlen(cmd))
		|| !ft_strncmp(cmd, "pwd", ft_strlen(cmd))
		|| !ft_strncmp(cmd, "export", ft_strlen(cmd))
		|| !ft_strncmp(cmd, "unset", ft_strlen(cmd))
		|| !ft_strncmp(cmd, "env", ft_strlen(cmd))
		|| !ft_strncmp(cmd, "exit", ft_strlen(cmd)));
}

int	run_builtin(char **args, char ***env)
{
	if (!ft_strncmp(args[0], "pwd", ft_strlen(args[0])))
		return (ft_pwd());
	if (!ft_strncmp(args[0], "cd", ft_strlen(args[0])))
		return (ft_cd(args, env));
	if (!ft_strncmp(args[0], "export", ft_strlen(args[0])))
		return (ft_export(args, env));
	if (!ft_strncmp(args[0], "unset", ft_strlen(args[0])))
		return (ft_unset(args, env));
	if (!ft_strncmp(args[0], "env", ft_strlen(args[0])))
		return (ft_env(env));
	if (!ft_strncmp(args[0], "exit", ft_strlen(args[0])))
		return (ft_exit(env));
	if (!ft_strncmp(args[0], "echo", ft_strlen(args[0])))
		return (ft_echo(args));
	return (1);
}
