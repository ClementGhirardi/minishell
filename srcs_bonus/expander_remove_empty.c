/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_remove_empty.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:27:11 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 16:27:36 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell.h"

static int	condition(char *test, char c)
{
	return ((test && *test) || (test && !*test && (c == '\'' || c == '"')));
}

static void	init_var(char ***clean, int *i, int *j)
{
	*clean = NULL;
	*i = -1;
	*j = 0;
}

char	**remove_empty_var(char **args, int status, char **env)
{
	char	**clean;
	char	**tmp;
	char	*test;
	int		i;
	int		j;

	init_var(&clean, &i, &j);
	while (args && args[++i])
	{
		test = expand_string(ft_strdup(args[i]), status, env);
		if (condition(test, *args[i]))
			j++;
		tmp = ft_realloc((void **)clean, sizeof(char *) * (j + 1));
		if (!tmp)
			return (free_array(args), free_array(clean), NULL);
		clean = tmp;
		if (condition(test, *args[i]))
			clean[j - 1] = ft_strjoin_and_free(clean[j - 1], args[i]);
		else
			free(args[i]);
		clean[j] = NULL;
		free(test);
	}
	return (free(args), clean);
}
