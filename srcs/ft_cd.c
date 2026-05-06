/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:50:19 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/06 16:22:54 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_pwd(char ***env)
{
	char	*old_pwd;
	char	cwd[4096];

	old_pwd = ft_getenv(*env, "PWD");
	if (old_pwd)
		ft_setenv(env, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_setenv(env, "PWD", cwd);
}

int	ft_cd(char **args, char ***env)
{
	char	*path;

	if (!args[1] || !ft_strncmp(args[1], "~", ft_strlen(args[1])))
	{
		path = ft_getenv(*env, "HOME");
		if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), 1);
	}
	else if (!ft_strncmp(args[1], "-", ft_strlen(args[1])))
	{
		path = ft_getenv(*env, "OLDPWD");
		if (!path)
			return (ft_putendl_fd("minishell: cd: OLDPWD not set", 2), 1);
		ft_putstr_fd(path, 1);
		ft_putchar_fd('\n', 1);
	}
	else
		path = args[1];
	if (chdir(path) == -1)
		return (ft_putstr_fd("minishell: cd: ", 2),
			ft_putstr_fd(path, 2),
			ft_putendl_fd(": no such file or directory", 2), 1);
	update_pwd(env);
	return (0);
}
