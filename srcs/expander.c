/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:41:43 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/15 15:43:02 by cghirard         ###   ########.fr       */
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

char	*question_mark(char *str, int *i, char **env)
{
	char	*tmp;
	char	*var;
	int		start;

	(*i)++;
	tmp = ft_itoa(status);
	var = ft_strdup(tmp);
	free(tmp);
	start = *i;
	while ((str[*i]))
		(*i)++;
	return (ft_strjoin_and_free(var, expand_string(
				ft_substr(str, start, *i - start), env)));
}

char	*extract_var_name(char *str, int *i, char **env)
{
	int		start;
	char	*tmp;
	char	*var;

	if (!str[++(*i)])
		return (ft_strdup(str));
	start = *i;
	if (str[*i] == '?')
		return (question_mark(str, i, env));
	if (!is_var_char(str[*i]))
		return (ft_strdup("$"));
	while (is_var_char(str[*i]))
		(*i)++;
	tmp = ft_substr(str, start, *i - start);
	if (!tmp)
		return (NULL);
	var = ft_getenv(env, tmp);
	free(tmp);
	return (var);
}

static char	*expand_quotes(char *str, int *i, char **env)
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
			var = extract_var_name(str, i, env);
		else
			var = ft_substr(str, (*i)++, 1);
		if (!var)
			return (free(tmp), NULL);
		tmp = ft_strjoin_and_free(tmp, var);
	}
	if (str[*i])
		(*i)++;
	return (tmp);
}

static char	*suppress_quotes(char *str)
{
	char	*new;
	int		i;
	int		start;
	char	quote;

	if (!str)
		return (NULL);
	new = NULL;
	quote = str[0];
	i = 1;
	while (str[i])
	{
		start = i;
		while (str[i] && str[i] != quote)
			i++;
		new = ft_strjoin_and_free(new, ft_substr(str, start, i - start));
		if (!new)
			return (NULL);
		if (str[i])
			i++;
	}
	return (new);
}

static char	*expand_quotes_first(char *str, int *i, char **env)
{
	char	quote;
	char	*tmp;
	char	*var;
	int		start;
	int		j;

	quote = str[(*i)++];
	start = *i;
	j = 0;
	tmp = ft_strdup("");
	if (!tmp)
		return (NULL);
	str = suppress_quotes(str);
	while (str[*i] && (str[*i] == '\'' || str[*i] == '"'))
		(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	tmp = ft_substr(str, start, *i - start);
	if (quote == '"')
		var = extract_var_name(tmp, &j, env);
	tmp = ft_strjoin_and_free(ft_substr(str, 0, start), var);
	tmp = ft_strjoin_and_free(tmp, ft_substr(str, *i, ft_strlen(str) - *i));
	return (tmp);
}

char	*expand_string(char *str, char **env)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	if (str[0] == '\'' || str[0] == '\"')
	{
		tmp = expand_quotes_first(str, &i, env);
		return (free(str), tmp);
	}
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			tmp = expand_quotes(str, &i, env);
		else if (str[i] == '$')
			tmp = extract_var_name(str, &i, env);
		else
			tmp = ft_substr(str, i++, 1);
		if (!tmp)
			return (free(str), free(result), NULL);
		result = ft_strjoin_and_free(result, tmp);
	}
	return (free(str), result);
}

// char	*expand_string(char *str, char **env)
// {
// 	char	*result;
// 	char	*tmp;
// 	int		i;

// 	result = ft_strdup("");
// 	if (!result)
// 		return (NULL);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '\'' || str[i] == '\"')
// 			tmp = expand_quotes(str, &i, env);
// 		else if (str[i] == '$')
// 			tmp = extract_var_name(str, &i, env);
// 		else
// 			tmp = ft_substr(str, i++, 1);
// 		if (!tmp)
// 			return (free(str), free(result), NULL);
// 		result = ft_strjoin_and_free(result, tmp);
// 	}
// 	return (free(str), result);
// }

void	expander(t_ast *node, char **env)
{
	char	*tmp;

	if (node && node->args)
		node->args = filter_and_dup_array(node->args, env);
	if (node && node->file)
	{
		tmp = expand_dollar_in_filename(ft_strdup(node->file), env);
		if (tmp)
		{
			node->file = expand_dollar_in_filename(node->file, env);
			if (node->file && !ft_strncmp(node->file, "", ft_strlen(node->file)))
			{
				free(node->file);
				node->file = NULL;
			}
			free(tmp);
		}
	}
}
