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

int	dollar_finder(char *file)
{
	int	i;

	i = 0;
	while (file[i])
	{
		if (file[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

char	*expand_dollar_in_filename(char *file, char **env)
{
	char	*filename;
	char	*tmp;
	int		i;
	int		start;

	i = 0;
	filename = NULL;
	while (file[i])
	{
		start = i;
		if (file[i] == '$')
			i++;
		while (file[i] && file[i] != '$')
			i++;
		tmp = ft_substr(file, start, i - start);
		if (!tmp)
			return (NULL);
		tmp = expand_string(tmp, env);
		filename = ft_strjoin_and_free(filename, tmp);
	}
	return (filename);
}

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
	(*i)++;
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

void	expander(t_ast *node, char **env)
{
	char	**new;

	if (node && node->args)
	{
		new = filter_and_dup_array(node->args, env);
		free(node->args);
		node->args = new;
	}
	if (node && node->file)
	{
		if (dollar_finder(node->file)
			&& !expand_dollar_in_filename(node->file, env))
			return ;
		node->file = expand_dollar_in_filename(node->file, env);
		if (node->file && !ft_strncmp(node->file, "", ft_strlen(node->file)))
		{
			free(node->file);
			node->file = NULL;
		}
	}
}
