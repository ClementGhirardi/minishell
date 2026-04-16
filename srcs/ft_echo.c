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

void	ft_display(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
	}
}

int	ft_echo(char **args)
{
	if (!args[1])
		return (ft_printf("\n"), 0);
	if (!ft_strncmp(args[1], "-n", ft_strlen(args[1])))
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
