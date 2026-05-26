/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:51:01 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 14:07:56 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_quotes(char *str, int *i)
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
		var = ft_substr(str, (*i)++, 1);
		if (!var)
			return ((*i)++, free(tmp), NULL);
		tmp = ft_strjoin_and_free(tmp, var);
	}
	if (str[*i])
		(*i)++;
	return (tmp);
}

char	*expand_file(char *str)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (free(str), NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			tmp = expand_quotes(str, &i);
		else
			tmp = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, tmp);
	}
	return (free(str), result);
}

t_ast	*create_redir_node(int *status, char **env, t_data *data)
{
	t_ast	*node;

	(void)status;
	(void)env;
	node = malloc(1 * sizeof(t_ast));
	if (!node)
		return (NULL);
	node->file = ft_strdup((*data->tokens)->value);
	if (!node->file)
		return (free(node), NULL);
	return (node);
}
