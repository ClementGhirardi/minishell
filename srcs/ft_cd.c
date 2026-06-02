/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:50:19 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 16:49:57 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_pwd(char ***env)
{
	char	*old_pwd;
	char	cwd[4096];

	old_pwd = ft_getenv(*env, "PWD");
	if (old_pwd)
		ft_setenv(env, "OLDPWD", old_pwd);
	free(old_pwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_setenv(env, "PWD", cwd);
}

int	ft_cd(char **args, char ***env, int fd_out)
{
	char	*path;

	if (!args[1] || !ft_strncmp(args[1], "~", ft_strlen(args[1])))
	{
		path = ft_getenv(*env, "HOME");
		if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", fd_out), 1);
	}
	else if (!ft_strncmp(args[1], "-", ft_strlen(args[1])))
	{
		path = ft_getenv(*env, "OLDPWD");
		if (!path)
			return (ft_putendl_fd("minishell: cd: OLDPWD not set", fd_out), 1);
		ft_putstr_fd(path, fd_out);
		ft_putchar_fd('\n', fd_out);
	}
	else
		path = ft_strdup(args[1]);
	if (chdir(path) == -1)
		return (ft_putstr_fd("minishell: cd: ", fd_out),
			ft_putstr_fd(path, fd_out),
			ft_putendl_fd(": no such file or directory", fd_out),
			free(path), 1);
	update_pwd(env);
	return (free(path), 0);
}
