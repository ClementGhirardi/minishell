/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 23:55:50 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/31 00:30:50 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	find_env_index(char **env, const char *name)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, name_len) && env[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	**dup_env_with_new_entry(char **env, const char *new_entry)
{
	char	**new_env;
	int		size;
	int		i;

	size = 0;
	while (env[size])
		size++;
	new_env = malloc((size + 2) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			return (free_array(new_env), NULL);
		i++;
	}
	new_env[i] = ft_strdup(new_entry);
	if (!new_env[i])
		return (free_array(new_env), NULL);
	new_env[i + 1] = NULL;
	return (new_env);
}

int	ft_setenv(char ***env, const char *name, const char *value)
{
	char	*new_entry;
	char	*tmp;
	int		index;
	char	**new_env;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (1);
	new_entry = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_entry)
		return (1);
	index = find_env_index(*env, name);
	if (index != -1)
		return (free((*env)[index]), (*env)[index] = new_entry, 0);
	else
	{
		new_env = dup_env_with_new_entry(*env, new_entry);
		free(new_entry);
		if (!new_env)
			return (1);
		free_array(*env);
		*env = new_env;
	}
	return (0);
}
