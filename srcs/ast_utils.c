/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:51:01 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/06 16:22:06 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static char	*ft_typetostr(t_token_type type)
// {
// 	if (type == TOKEN_PIPE)
// 		return (ft_strdup("`|'"));
// 	if (type == TOKEN_REDIR_IN)
// 		return (ft_strdup("`<'"));
// 	if (type == TOKEN_REDIR_OUT)
// 		return (ft_strdup("`>'"));
// 	if (type == TOKEN_APPEND)
// 		return (ft_strdup("`>>'"));
// 	if (type == TOKEN_HEREDOC)
// 		return (ft_strdup("`<<'"));
// 	return (NULL);
// }

t_ast	*create_redir_node(char **env, t_data *data)
{
	t_ast	*node;
	//char	*str;

	// if (!(*data->tokens)->value)
	// 	return (error_syntax("`newline'"), NULL);
	node = malloc(1 * sizeof(t_ast));
	// if (!node)
	// 	return (error_syntax("`newline'"), NULL);
	// if ((*data->tokens)->type != TOKEN_WORD)
	// {
	// 	str = ft_typetostr((*data->tokens)->type);
	// 	error_syntax(str);
	// 	if (str)
	// 		free(str);
	// 	return (free(node), NULL);
	// }
	node->file = ft_strdup((*data->tokens)->value);
	node->file = expand_string(node->file, env);
	if (!node->file)
		return (free(node), NULL);
	return (node);
}
