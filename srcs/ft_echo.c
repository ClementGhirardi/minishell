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

int	safe_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 && !s2)
		return (0);
	if (s1 && !s2)
		return (*s1);
	if (!s1 && s2)
		return (-(*s2));
	i = -1;
	while (s1[++i])
	{
		if (s1[i] - s2[i])
			return (s1[i] - s2[i]);
	}
	return (s1[i] - s2[i]);
}

void	ft_display(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (!args[i][0])
			ft_printf(" ");
		else
		{
			ft_printf("%s", args[i]);
			if (args[i + 1] && args[i + 1][0])
				ft_printf(" ");
		}
	}
}

int	ft_echo(char **args)
{
	if (!args[1])
		return (ft_printf("\n"), 0);
	if (!safe_strcmp(args[1], "-n"))
	{
		if (!args[2])
			return (0);
		ft_display(&args[2]);
	}
	else
	{
		ft_display(&args[1]);
		ft_printf("\n");
	}
	return (0);
}
