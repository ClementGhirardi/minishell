/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 13:59:59 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 14:08:23 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*ft_getenv(char **env, const char *name)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, name_len)
			&& (env[i][name_len] == '='
			|| ft_strlen(env[i]) == ft_strlen(name)))
			return (get_value(env[i]));
		i++;
	}
	return (NULL);
}
