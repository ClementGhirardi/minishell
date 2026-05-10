/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbarth <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 14:29:14 by adbarth           #+#    #+#             */
/*   Updated: 2026/04/09 14:29:23 by adbarth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_first_arg(char *s)
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

void	ft_display(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
	{
		if (!args[i][0])
			ft_printf(" ");
		else
		{
			ft_printf("%s", args[i]);
			if (args[i + 1] && args[i + 1][0])
				ft_printf(" ");
		}
		i++;
	}
}

int	ft_echo(char **args)
{
	int	i;

	i = 2;
	if (!args || !*args || !args[1])
		return (ft_printf("\n"), 0);
	if (check_first_arg(args[1]))
	{
		if (!args[2])
			return (0);
		while (check_first_arg(args[i]))
			i++;
		if (!args[i])
			return (0);
		ft_display(&args[i]);
	}
	else
	{
		ft_display(&args[1]);
		ft_printf("\n");
	}
	return (0);
}
