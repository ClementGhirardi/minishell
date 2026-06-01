/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_nb_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 11:44:08 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/01 12:02:58 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	recursive(t_ast *ast, int *count)
{
	if (!ast)
		return ;
	if (ast->type == NODE_HEREDOC)
		(*count)++;
	recursive(ast->left, count);
	recursive(ast->right, count);
}

void	check_nb_heredoc(t_ast *ast, t_data *data)
{
	int	count;

	count = 0;
	recursive(ast, &count);
	if (count > 16)
	{
		ft_putendl_fd("minishell: maximum here-document count exceeded", 2);
		free(*data->input);
		free_array(data->env);
		ast_free(ast);
		exit(2);
	}
}
