/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:41:43 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/13 12:27:12 by cghirard         ###   ########.fr       */
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

char	*extract_var_name(char *str, int *i, int status, char **env)
{
	int		start;
	char	*tmp;
	char	*var;

	(*i)++;
	start = *i;
	if (str[*i] == '?')
		return ((*i)++, ft_strdup(ft_itoa(status)));
	if (str[*i] == '0')
		return ((*i)++, ft_getenv(env, "SHELL"));
	if ('1' <= str[*i] && str[*i] <= '9')
		return ((*i)++, NULL);
	if (!is_var_char(str[*i]))
		return (ft_strdup("$"));
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

static char	*expand_quotes(char *str, int *i, int status, char **env)
{
	char	quote;
	char	*tmp;
	char	*var;

	quote = str[(*i)++];
	tmp = ft_strdup("");
	if (!tmp)
		return (NULL);
	while (str[*i] && str[*i] != quote)
	{
		if (quote != '\'' && str[*i] == '$')
			var = extract_var_name(str, i, status, env);
		else
			var = ft_substr(str, (*i)++, 1);
		if (!var)
			return (free(tmp), NULL);
		tmp = ft_strjoin_and_free(tmp, var);
	}
	(*i)++;
	return (tmp);
}

char	*expand_string(char *str, int status, char **env)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			tmp = expand_quotes(str, &i, status, env);
		else if (str[i] == '$')
			tmp = extract_var_name(str, &i, status, env);
		else
			tmp = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, tmp);
	}
	return (free(str), result);
}

void	expander(t_ast *node, int status, char **env)
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
