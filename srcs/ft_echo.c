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

int	ft_display(char **args, char ***env)
{
	char	*word;
	int		status;
	int		i;

	status = 0;
	i = -1;
	while (args[++i])
	{
		word = expand_string(ft_strdup(args[i]), status, env);
		if (word)
		{
			ft_printf("%s", word);
			free(word);
			if (args[i + 1] && args[i][ft_strlen(args[i]) - 1] != '\'' && args[i][ft_strlen(args[i]) - 1] != '"')
				ft_printf(" ");
		}
		else
			return (1);
	}
	return (0);
}

int	ft_echo(char **args, char ***env)
{
	int	status;

	if (!ft_strncmp(args[1], "-n", ft_strlen(args[1])))
		status = ft_display(&args[2], env);
	else
	{
		status = ft_display(&args[1], env);
		ft_printf("\n");
	}
	return (status);
}
