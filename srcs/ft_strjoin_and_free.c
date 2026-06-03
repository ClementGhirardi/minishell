/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_and_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:53:13 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 11:53:31 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*tmp;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		tmp = ft_strdup(s2);
	else if (s1 && !s2)
		tmp = ft_strdup(s1);
	else
		tmp = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (tmp);
}

int	ft_is_in(char c, char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 && !s2)
		return (0);
	if (!s2)
		return (s1[0]);
	if (!s1)
		return (-s2[0]);
	while (s1[i])
	{
		if (s1[i] - s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}
