/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:13:26 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/12 13:37:59 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_id(char *arg)
{
	int	i;

	i = 0;
	if (!(('a' <= arg[i] && arg[i] <= 'z')
			|| ('A' <= arg[i] && arg[i] <= 'Z')
			|| (arg[i] == '_')))
		return (ft_putstr_fd("minishells: export: `", 2),
			ft_putstr_fd(arg, 2),
			ft_putendl_fd("': not a valid identifier", 2), 0);
	while (arg[i])
	{
		if (!(('a' <= arg[i] && arg[i] <= 'z')
				|| ('A' <= arg[i] && arg[i] <= 'Z')
				|| ('0' <= arg[i] && arg[i] <= '9')
				|| (arg[i] == '_') || (arg[i] == '=')))
			return (ft_putstr_fd("minishell: export: `", 2),
				ft_putstr_fd(arg, 2),
				ft_putendl_fd("': not a valid identifier", 2), 0);
		i++;
	}
	return (1);
}

static char	*get_name(char *arg)
{
	int		len;
	char	*name;

	len = 0;
	if (arg[0] == '=')
	{
		while (arg[len])
			len++;
	}
	else
	{
		while (arg[len] && arg[len] != '=')
			len++;
	}
	name = ft_substr(arg, 0, len);
	if (!name)
		return (NULL);
	if (!is_valid_id(name))
		return (free(name), NULL);
	return (name);
}

static char	*get_value(char *arg)
{
	int		start;
	int		len;
	char	*value;

	start = 0;
	while (arg[start] && arg[start] != '=')
		start++;
	if (arg[start] != '=')
		return (NULL);
	start++;
	len = ft_strlen(arg);
	value = ft_substr(arg, start, len);
	if (!value)
		return (NULL);
	return (value);
}

static int	print_export(char **env, int fd_out)
{
	int		i;
	char	*name;
	char	*value;

	i = 0;
	while (env[i])
	{
		name = get_name(env[i]);
		if (!name)
			return (1);
		ft_putstr_fd("export ", fd_out);
		ft_putstr_fd(name, fd_out);
		// ft_putstr_fd("export ", 1);
		// ft_putstr_fd(name, 1);
		value = get_value(env[i]);
		if (value)
		{
			ft_putstr_fd("=\"", fd_out);
			ft_putstr_fd(value, fd_out);
			ft_putstr_fd("\"", fd_out);
			// ft_putstr_fd("=\"", 1);
			// ft_putstr_fd(value, 1);
			// ft_putstr_fd("\"", 1);
			free(value);
		}
		ft_putendl_fd("", fd_out); // ft_putendl_fd("", 1);
		free(name);
		i++;
	}
	return (0);
}

int	ft_export(char **args, char ***env, int status, int fd_out)
{
	int		i;
	char	*name;
	char	*value;

	if (!args[1])
		return (print_export(sort_array(*env), fd_out));
	i = 1;
	while (args[i])
	{
		name = get_name(args[i]);
		if (!name)
			status = 1;
		value = get_value(args[i]);
		if (ft_setenv(env, name, value))
		{
			if (value)
				free(value);
			return (free(name), 1);
		}
		free(name);
		if (value)
			free(value);
		i++;
	}
	return (status);
}
