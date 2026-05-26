/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:51:01 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 17:26:19 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_typetostr(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (ft_strdup("`|'"));
	if (type == TOKEN_REDIR_IN)
		return (ft_strdup("`<'"));
	if (type == TOKEN_REDIR_OUT)
		return (ft_strdup("`>'"));
	if (type == TOKEN_APPEND)
		return (ft_strdup("`>>'"));
	if (type == TOKEN_HEREDOC)
		return (ft_strdup("`<<'"));
	return (NULL);
}

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
			return (free(tmp), NULL);
		tmp = ft_strjoin_and_free(tmp, var);
	}
	(*i)++;
	return (tmp);
}

static char	*expand_only_quotes(char *str)
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
			tmp = expand_quotes(str, &i);
		else
			tmp = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, tmp);
	}
	return (free(str), result);
}

t_ast	*create_redir_node(t_token_type r_type, int *status, char **env,
		t_token *tokens)
{
	t_ast	*node;
	char	*str;

	if (!tokens->value)
		return (error_syntax("`newline'", status), NULL);
	node = malloc(1 * sizeof(t_ast));
	if (!node)
		return (error_syntax("`newline'", status), NULL);
	if (tokens->type != TOKEN_WORD)
	{
		str = ft_typetostr(tokens->type);
		error_syntax(str, status);
		if (str)
			free(str);
		return (free(node), NULL);
	}
	node->file = ft_strdup(tokens->value);
	if (r_type == TOKEN_HEREDOC)
		node->file = expand_only_quotes(node->file);
	else
		node->file = expand_string(node->file, *status, env);
	if (!node->file)
		return (free(node), NULL);
	return (node);
}
