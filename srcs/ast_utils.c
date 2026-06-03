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

t_ast	*create_redir_node(t_token_type r_type, int *status, char **env,
		t_token *tokens)
{
	t_ast	*node;

	(void)env;
	(void)status;
	(void)r_type;
	node = malloc(1 * sizeof(t_ast));
	node->file = ft_strdup(tokens->value);
	if (!node->file)
		return (free(node), NULL);
	return (node);
}
