/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:54:39 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/02 18:10:55 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
			ft_printf("|%s|", node->args[i]);
			i++;
			if (node->args[i])
				ft_printf(", ");
		}
	}
	if (node->file)
	{
		ft_printf("-%s-: ", node->file);
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

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	init_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int ac, char **av, char **env)
{
	char	*input;

	(void)ac;
	(void)av;
	(void)env;
	init_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		// Tests begin
		ft_printf("--TEST LEXER--\n");
		t_token *tokens = lexer(input);
		while (tokens)
		{
			ft_printf("%d: %s\n", tokens->type, tokens->value);
			tokens = tokens->next;
		}
		ft_printf("\n");
		ft_printf("--TEST PARSER--\n");
		t_ast	*node = parse(lexer(input));
		ast_show(node);
		ft_printf("\n\n");
		ft_printf("--TEST EXPANDER--\n");
		expander(node);
		ast_show(node);
		ft_printf("\n\n");
		// Tests end
		free(input);
	}
	rl_clear_history();
	return (0);
}
