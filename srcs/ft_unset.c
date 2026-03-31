/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:14:03 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 00:01:12 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**get_new_env(char **env)
{
	int		size;
	char	**new_env;

	size = 0;
	while (env[size])
		size++;
	new_env = malloc(size * sizeof(char *));
	return (new_env);
}

static char	**remove_env_entry(char **env, const char *name)
{
	int		i;
	int		j;
	char	**new_env;

	new_env = get_new_env(env);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, ft_strlen(name))
			|| !(ft_strlen(name) == ft_strlen(env[i])
				|| env[i][ft_strlen(name)] == '='))
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j])
				return (free_array(new_env), NULL);
			j++;
		}
		i++;
	}
	return (new_env[j] = NULL, new_env);
}

int	ft_unset(char **args, char ***env)
{
	char	**new_env;
	int		i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		new_env = remove_env_entry(*env, args[i]);
		if (!new_env)
			return (1);
		free_array(*env);
		*env = new_env;
		i++;
	}
	return (0);
}
