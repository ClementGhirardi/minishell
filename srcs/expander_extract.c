/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_extract.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:26:35 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 16:39:56 by cghirard         ###   ########.fr       */
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

static char	*ft(char *str, int start, int *i, char **env)
{
	char	*tmp;
	char	*var;

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

char	*extract_quotevar_name(char *str, int *i, int status, char **env)
{
	int		start;
	char	*tmp;
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
	return (ft(str, start, i, env));
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
		while (is_var_char(str[++(*i)]))
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
