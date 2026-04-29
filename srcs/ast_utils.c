/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:51:01 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/28 15:52:47 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_typetostr(t_token_type type)
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

t_ast	*create_redir_node(int *status, t_data *data)
{
	t_ast	*node;
	char	*str;

	node = malloc(1 * sizeof(t_ast));
	if (!node || !(*data->tokens)->value)
		return (error_syntax("`newline'", status), NULL);
	if ((*data->tokens)->type != TOKEN_WORD)
	{
		str = ft_typetostr((*data->tokens)->type);
		error_syntax(str, status);
		if (str)
			free(str);
		return (NULL);
	}
	node->file = ft_strdup((*data->tokens)->value);
	if (!node->file)
		return (free(node), NULL);
	return (node);
}
