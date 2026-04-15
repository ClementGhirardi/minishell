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

// char	*remove_quotes(char *word)
// {
// 	char	*result;
// 	int		i;
// 	char	quote;

// 	i = 0;
// 	while ()

// }

int	ft_display(char **args, char ***env)
{
	char	*word;
	int		status;
	int		i;

	word = NULL;
	status = 0;
	i = -1;
	while (args[++i])
		word = ft_strjoin_and_free(word, expand_string(ft_strdup(args[i]), status, env));
	//word = remove_quotes(word);
	if (word)
	{
		//ft_printf("%s", word);
		free(word);
		if (args[i + 1])
			ft_printf(" ");
	}
	else
		return (1);
	return (0);
}

int	ft_echo(char **args, char ***env)
{
	int	status;

	if (!ft_strncmp(args[1], "-n", ft_strlen(args[1]))) //erreur si !args[1]
		status = ft_display(&args[2], env);
	else
	{
		status = ft_display(&args[1], env);
		ft_printf("\n");
	}
	return (status);
}
