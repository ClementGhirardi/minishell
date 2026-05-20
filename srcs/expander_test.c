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

// char	*extract_var_name(char *str, int *i, int status, char **env)
// {
// 	int		start;
// 	char	*tmp;
// 	char	*var;
// 	char	*str_status;

// 	(*i)++;
// 	start = *i;
// 	if (str[*i] == '?')
// 	{
// 		str_status = ft_itoa(status);
// 		if (!str_status)
// 			return ((*i)++, NULL);
// 		tmp = ft_strdup(str_status);
// 		return ((*i)++, free(str_status), tmp);
// 	}
// 	while (is_var_char(str[*i]))
// 		(*i)++;
// 	tmp = ft_substr(str, start, *i - start);
// 	if (!tmp)
// 		return (NULL);
// 	var = ft_getenv(env, tmp);
// 	free(tmp);
// 	if (!var)
// 		return (NULL);
// 	return (var);
// }


char	*extract_var_name(char *str, int *i, int status, char **env)
{
	int		start;
	char	*tmp;
	char	*var;
	char	*str_status;

	start = (*i)++;
	if (str[*i] == '?')
	{
		str_status = ft_itoa(status);
		if (!str_status)
			return ((*i)++, NULL);
		tmp = ft_strdup(str_status);
		while (str[++(*i)])
			// tmp = ft_strjoin_and_free(tmp, expand_string(ft_strdup(&str[*i]), status, env));
			tmp = ft_strjoin_and_free(tmp, ft_substr(str, (*i), 1));
		ft_printf("tmp a la fin de extractvarname = %s\n", tmp);
		return (free(str_status), tmp);
	}
	while (is_var_char(str[*i]))
		(*i)++;
	tmp = ft_substr(str, start, *i - start);
	if (!tmp)
		return (NULL);
	if (!tmp[1] || tmp[1] == '$')
		return (free(tmp), *i = ft_strlen(str), ft_strdup(str));
	var = ft_getenv(env, tmp);
	ft_printf("var = %s\n", var);
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
	ft_printf("\nstrlen de str = %d\n", ft_strlen(str));
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			tmp = expand_quotes(str, &i, status, env);
		else if (str[i] == '$')
			tmp = extract_var_name(str, &i, status, env);
		else
			tmp = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, tmp);
		if (!str[i])
			ft_printf("str[i] == caractere null\n");
		ft_printf("str[i] = %c\n", str[i]);
		ft_printf("i = %d\n", i);
	}
	return (free(str), str = NULL, result);
}

void	*ft_realloc(void *ptr, size_t size)
{
	unsigned char	*new;
	unsigned char	*p;
	size_t			i;

	if (!size)
		return (free(ptr), NULL);
	p = ptr;
	i = 0;
	new = ft_calloc(1, size);
	if (!new)
		return (NULL);
	while (p && p[i])
	{
		new[i] = p[i];
		i++;
	}
	free(ptr);
	return (new);
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
		ft_printf("strlen de args[i] (avant expandstring) = %d\n", ft_strlen(args[i]));
		test = expand_string(ft_strdup(args[i]), status, env);
		if (test && *test)
			j++;
		tmp = ft_realloc((void *)clean, sizeof(char *) * (j + 1));
		if (!tmp)
			return (free_array(args), free_array(clean), NULL);
		clean = tmp;
		if (test && *test)
			clean[j - 1] = ft_strjoin_and_free(clean[j - 1], args[i]);
			//clean[j - 1] = ft_strjoin_and_free(clean[j - 1], expand_string(args[i], status, env));
		else
			free(args[i]);
		//ft_printf("strlen de args[i] (apres expandstring) = %d\n", ft_strlen(args[i]));
		clean[j] = NULL;
		free(test);
		test = NULL;
	}
	return (free(args), clean);
}

void	expander(t_ast *node, int status, char **env)
{
	char	*tmp;
	int		i;

	if (node && node->args)
	{
		i = -1;
		node->args = remove_empty_var(node->args, status, env);
		while (node->args && node->args[++i])
		{
			node->args[i] = expand_string(node->args[i], status, env);
			ft_printf("OK\n");
		}
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
