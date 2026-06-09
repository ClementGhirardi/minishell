/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 14:29:14 by adbarth           #+#    #+#             */
/*   Updated: 2026/06/03 13:49:18 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell.h"

static int	check_first_arg(char *s)
{
	int	i;

	i = 2;
	if (!s)
		return (0);
	if (s[0] != '-' || s[1] != 'n')
		return (0);
	while (s && s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	ft_display(char **args, int fd_out)
{
	int	i;

	i = 0;
	while (args && args[i])
	{
		if (!args[i][0] && i > 0)
			ft_putstr_fd(" ", fd_out);
		else
		{
			ft_putstr_fd(args[i], fd_out);
			if (args[i + 1] && args[i + 1][0])
				ft_putstr_fd(" ", fd_out);
		}
		i++;
	}
}

int	ft_echo(char **args, int fd_out)
{
	int	i;

	i = 2;
	if (!args || !*args || !args[1])
		return (ft_putstr_fd("\n", fd_out), 0);
	if (check_first_arg(args[1]))
	{
		if (!args[2])
			return (0);
		while (check_first_arg(args[i]))
			i++;
		if (!args[i])
			return (0);
		ft_display(&args[i], fd_out);
	}
	else
	{
		ft_display(&args[1], fd_out);
		ft_putstr_fd("\n", fd_out);
	}
	return (0);
}
