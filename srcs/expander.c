/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:41:43 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/02 17:23:29 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_var_char(char c)
{
	if (('a' <= c && c <= 'z')
		|| ('A' <= c && c <= 'Z')
		|| ('0' <= c && c <= '9')
		|| (c == '_'))
		return (1);
	return (0);
}

static char	*extract_var_name(char *str, int *i, int status, char ***env)
{
	int		start;
	char	*tmp;
	char	*var;

	(*i)++;
	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		var = ft_strdup(ft_itoa(status));
		return (var);
	}
	while (is_var_char(str[*i]))
		(*i)++;
	tmp = ft_substr(str, start, *i - start);
	if (!tmp)
		return (NULL);
	var = ft_getenv(env, tmp);
	if (!var)
		return (NULL);
	free(tmp);
	return (var);
}

char	*expand_string(char *str, int status, char ***env)
{
	int		i;
	char	*result;
	char	*var;

	if (str[0] == '\'')
		return (result = ft_substr(str, 1, ft_strlen(str) - 2),
			free(str), result);
	i = 0;
	result = ft_strdup("");
	if (!result)
		return (free(str), NULL);
	while (str[i])
	{
		if (str[i] == '$')
			var = extract_var_name(str, &i, status, env);
		else
			var = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, var);
	}
	if (str[0] == '\"')
		return (free(str), str = ft_substr(result, 1, ft_strlen(result) - 2),
			free(result), str);
	return (free(str), result);
}

void	expander(t_ast *node, int status, char ***env)
{
	int	i;

	if (!node)
		return ;
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			node->args[i] = expand_string(node->args[i], status, env);
			i++;
		}
	}
	if (node->file)
	{
		node->file = expand_string(node->file, status, env);
		if (node->file && !ft_strncmp(node->file, "", ft_strlen(node->file)))
		{
			free(node->file);
			node->file = NULL;
		}
	}
}
