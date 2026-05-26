/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:31:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 13:47:47 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	error_num(char *arg, int *status)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	*status = 2;
}

static void	error_too_many_args(int *status)
{
	ft_putendl_fd("minishell: exit: too many arguments", 2);
	*status = 1;
}

static int	ft_strslen(char **strs)
{
	int	len;

	len = 0;
	if (!strs)
		return (len);
	while (strs[len])
		len++;
	return (len);
}

static int	ft_is_str_digit(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, int status)
{
	int	len;

	if (!args)
		return (status);
	len = ft_strslen(args);
	if (len >= 2)
	{
		if (!ft_is_str_digit(args[1]))
			error_num(args[1], &status);
		else if (len > 2)
			error_too_many_args(&status);
		else
			status = ft_atoi(args[1]);
	}
	return (status);
}
