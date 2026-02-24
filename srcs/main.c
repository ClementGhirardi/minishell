/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:54:39 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/24 12:45:14 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ast_show_type(t_ast *node)
{
	if (!node)
		return ;
	ft_printf("(%d", node->type);
	ast_show_type(node->left);
	if (node->right)
		ft_printf(", ");
	ast_show_type(node->right);
	ft_printf(")");
}

void	ast_show(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	if (node->type == NODE_PIPE)
		ft_printf("PIPE(");
	else if (node->type == NODE_CMD)
		ft_printf("CMD(");
	else
		ft_printf("REDIR(");
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			ft_printf("%s", node->args[i]);
			i++;
			if (node->args[i])
				ft_printf(", ");
		}
	}
	if (node->file)
	{
		ft_printf("\'%s\': ", node->file);
		ast_show(node->left);
	}
	else
	{
		ast_show(node->left);
		if (node->right)
			ft_printf(", ");
		ast_show(node->right);
	}
	ft_printf(")");
}

int	main(int ac, char **av)
{
	t_token	*tokens;
	t_ast	*node;

	(void)ac;
	tokens = lexer(av[1]);
	while (tokens)
	{
		ft_printf("%d: %s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
	ft_printf("\n_____________________\n\n");
	node = parse(lexer(av[1]));
	ast_show_type(node);
	ft_printf("\n_____________________\n\n");
	node = parse(lexer(av[1]));
	ast_show(node);
	return (0);
}
