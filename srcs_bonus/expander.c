/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:41:43 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 11:45:48 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell.h"

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
