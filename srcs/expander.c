/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:41:43 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/12 11:30:45 by cghirard         ###   ########.fr       */
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

char	*extract_quotevar_name(char *str, int *i, int status, char **env)
{
	int		start;
	char	*tmp;
	char	*var;
	char	*str_status;

	start = ++(*i);
	if (str[*i] == '?')
	{
		str_status = ft_itoa(status);
		if (!str_status)
			return ((*i)++, NULL);
		tmp = ft_strdup(str_status);
		while (str[++(*i)] && str[*i] != '$' && str[*i] != '"')
			tmp = ft_strjoin_and_free(tmp, ft_substr(str, (*i), 1));
		if (str[*i] == '"')
			(*i)++;
		return (free(str_status), tmp);
	}
	while (is_var_char(str[*i]))
		(*i)++;
	tmp = ft_substr(str, start, *i - start);
	if (!tmp)
		return (NULL);
	if (!tmp[0])
		return (free(tmp), ft_strdup("$"));
	var = ft_getenv(env, tmp);
	if (!var)
		return (free(tmp), ft_strdup(""));
	return (free(tmp), var);
}

char	*extract_var_name(char *str, int *i, int status, char **env)
{
	int		start;
	char	*tmp;
	char	*var;
	char	*str_status;

	start = ++(*i);
	if (str[*i] == '?')
	{
		str_status = ft_itoa(status);
		if (!str_status)
			return ((*i)++, NULL);
		tmp = ft_strdup(str_status);
		while (str[++(*i)] && str[*i] != '$')
			tmp = ft_strjoin_and_free(tmp, ft_substr(str, (*i), 1));
		return (free(str_status), tmp);
	}
	while (is_var_char(str[*i]))
		(*i)++;
	tmp = ft_substr(str, start, *i - start);
	if (!tmp)
		return (NULL);
	if (!tmp[0])
		return (free(tmp), ft_strdup("$"));
	var = ft_getenv(env, tmp);
	return (free(tmp), var);
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
			var = extract_quotevar_name(str, i, status, env);
		else
			var = ft_substr(str, (*i)++, 1);
		if (!var)
			return ((*i)++, free(tmp), NULL);
		tmp = ft_strjoin_and_free(tmp, var);
	}
	if (str[*i])
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
	return (free(str), str = NULL, result);
}

char	**ft_realloc(void **ptr, size_t size)
{
	unsigned char	**new;
	unsigned char	**p;
	size_t			i;
	size_t			j;

	if (!size)
		return (free_array((char **)ptr), NULL);
	p = (unsigned char **)ptr;
	i = 0;
	new = ft_calloc(1, size);
	if (!new)
		return (NULL);
	while (p && p[i])
	{
		new[i] = ft_calloc(1, sizeof(char) * (ft_strlen((const char *)p[i]) + 1));
		if (!new[i])
			return (NULL);
		j = -1;
		while (p[i][++j])
			new[i][j] = p[i][j];
		i++;
	}
	free_array((char **)ptr);
	return ((char **)new);
}

int	is_empty(char *str)
{
	char	*new;
	int		i;
	int 	start;
	char	quote;

	i = -1;
	while (str && str[++i])
	{
		if (str[i] != '\'' && str[i] != '"')
			return (0);
	}
	i = -1;
	while (str && str[++i])
	{
		quote = str[i];
		start = ++i;
		while (str[i] != quote)
			i++;
		new = ft_substr(str, start, i - quote);
		if (!new)
			return (0);
		if (!is_empty(new))
			return (0);
	}
	return (1);
}

char	**remove_empty_var(char **args, int status, char **env)
{
	char	**clean;
	char	**tmp;
	char	*test;
	int		i;
	int		j;

	clean = NULL;
	i = -1;
	j = 0;
	while (args && args[++i])
	{
		test = expand_string(ft_strdup(args[i]), status, env);
		if ((test && *test)
			|| (test && !*test 
				&& (*args[i] == '\'' || *args[i] == '"')))
			j++;
		tmp = ft_realloc((void **)clean, sizeof(char *) * (j + 1));
		if (!tmp)
			return (free_array(args), free_array(clean), NULL);
		clean = tmp;
		if ((test && *test)
			|| (test && !*test 
				&& (*args[i] == '\'' || *args[i] == '"')))
			clean[j - 1] = ft_strjoin_and_free(clean[j - 1], args[i]);
		else
			free(args[i]);
		clean[j] = NULL;
		free(test);
	}
	return (free(args), clean);
}

void	expander(t_ast *node, int status, char **env)
{
	char	*tmp;
	int		i;

	i = -1;
	if (node && node->args)
	{
		node->args = remove_empty_var(node->args, status, env);
		while (node->args && node->args[++i])
			node->args[i] = expand_string(node->args[i], status, env);
	}
	if (node && node->file)
	{
		tmp = ft_strdup(node->file);
		node->file = expand_string(node->file, status, env);
		if (node->file && !ft_strncmp(node->file, "", ft_strlen(node->file)))
		{
			if (ft_strchr(tmp, '$'))
				error_file(tmp);
			else
				error_open(NULL);
		}
		free(tmp);
	}
}
