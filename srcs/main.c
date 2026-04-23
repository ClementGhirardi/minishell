/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:54:39 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/02 15:43:33 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	status = 0;

// void	ast_show(t_ast *ast)
// {
// 	int	i;

// 	if (!ast)
// 		return ;
// 	if (ast->type == NODE_PIPE)
// 		ft_printf("PIPE(");
// 	else if (ast->type == NODE_OR)
// 		ft_printf("OR(");
// 	else if (ast->type == NODE_AND)
// 		ft_printf("AND(");
// 	else if (ast->type == NODE_CMD)
// 		ft_printf("CMD(");
// 	else if (ast->type == NODE_REDIR_IN)
// 		ft_printf("REDIR_IN(");
// 	else if (ast->type == NODE_REDIR_OUT)
// 		ft_printf("REDIR_OUT(");
// 	else if (ast->type == NODE_APPEND)
// 		ft_printf("APPEND(");
// 	else if (ast->type == NODE_HEREDOC)
// 		ft_printf("HEREDOC(");
// 	else
// 		ft_printf("REDIR(");
// 	if (ast->args)
// 	{
// 		i = 0;
// 		while (ast->args[i])
// 		{
// 			ft_printf("|%s|", ast->args[i]);
// 			i++;
// 			if (ast->args[i])
// 				ft_printf(", ");
// 		}
// 	}
// 	if (ast->type == NODE_REDIR_IN)
// 	{
// 		ft_printf("-%s-: ", ast->file);
// 		ast_show(ast->left);
// 	}
// 	else
// 	{
// 		ast_show(ast->left);
// 		if (ast->right)
// 			ft_printf(", ");
// 		ast_show(ast->right);
// 	}
// 	ft_printf(")");
// }

// // BEGIN TESTS
// // TEST LEXER
// t_token *tokens = lexer(input);
// ft_printf("--TEST LEXER--\n");
// t_token *current = tokens;
// while (current)
// {
// 	ft_printf("%d: |%s|\n", current->type, current->value);
// 	current = current->next;
// }
// ft_printf("\n");
// // TEST PARSER
// ft_printf("--TEST PARSER--\n");
// t_ast *ast = parse(tokens);
// ast_show(ast);
// ft_printf("\n\n");
// // END TESTS

void	sigint_handler(int sig) //STATIC
{
	(void)sig;
	status = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	init_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	minishell(char **input, char ***env)
{
	t_token	*tokens;
	t_token	*tmp;
	t_ast	*ast;

	tokens = lexer(input, *env);
	// tmp = tokens;
	// t_token *current = tokens;
	// while (current)
	// {
		// ft_printf("%d: |%s|\n", current->type, current->value);
		// current = current->next;
	// }
	tokens = split_bracket(&tokens);

	// tmp = tokens;
	// while (tmp)
	// {
	// 	if (tmp->bracket)
	// 		ft_printf("((");
	//  	ft_printf("%d: |%s|\n", tmp->type, tmp->value);
	// 	tmp = tmp->next;
	// }

	// free_token(tmp);
	tmp = tokens;
	// current = tokens;
	// while (current)
	// {
		// ft_printf("%d: |%s|\n", current->type, current->value);
		// current = current->next;
	// }
	if (tokens)
	{
		ast = parse(&tokens, *env, input);
		if (ast)
		{
			status = executor(ast, env);
			ast_free(ast);
		}
		free_token(tmp);
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**env;

	(void)ac;
	(void)av;
	env = dup_array(envp);
	if (!env)
		return (1);
	init_signals();
	while (1)
	{
		if (status == 130)
			status = 130;
		input = readline("minishell$ ");
		if (!input)
			ft_exit(&env);
		minishell(&input, &env); //&
		free(input);
	}
	ft_exit(&env);
}
