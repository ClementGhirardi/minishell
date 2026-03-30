/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:54:39 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/31 00:44:54 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	ast_show(t_ast *ast)
// {
// 	int	i;

// 	if (!ast)
// 		return ;
// 	if (ast->type == NODE_PIPE)
// 		ft_printf("PIPE(");
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
// 	if (ast->file)
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

static void	sigint_handler(int sig)
{
	(void)sig;
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

		// // Tests begin
		// ft_printf("--TEST LEXER--\n");
		// tokens = lexer(input);
		// while (tokens)
		// {
		// 	ft_printf("%d: %s\n", tokens->type, tokens->value);
		// 	tokens = tokens->next;
		// }
		// ft_printf("\n");
		// ft_printf("--TEST PARSER--\n");
		// ast = parse(lexer(input));
		// ast_show(ast);
		// ft_printf("\n\n");
		// ft_printf("--TEST EXECUTOR--\n");
		// status = executor(ast, status, &envp);
		// ft_printf("\n");
		// // Tests end

static void	minishell(int status, char *input, char **env)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = lexer(input);
	if (tokens)
	{
		ast = parse(tokens);
		if (ast)
		{
			status = executor(ast, status, &env);
			ast_free(ast);
		}
		free_token(tokens);
	}
	free(input);
}

int	main(int ac, char **av, char **envp)
{
	int		status;
	char	*input;
	char	**env;

	(void)ac;
	(void)av;
	env = dup_array(envp);
	status = 0;
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
		minishell(status, input, env);
	}
	rl_clear_history();
	free_array(env);
	return (status);
}
