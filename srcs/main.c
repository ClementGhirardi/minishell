/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:54:39 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/27 14:39:01 by cghirard         ###   ########.fr       */
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

volatile sig_atomic_t	g_sig_status = 0;

static void	sigint_handler(int sig)
{
	(void) sig;
	if (g_sig_status == -1)
		g_sig_status = 0;
	if (g_sig_status == 1)
	{
		g_sig_status = 2;
		ft_putchar_fd('\n', 1);
		return ;
	}
	if (g_sig_status == 3)
	{
		g_sig_status = 4;
		exit(130);
	}
	ft_putendl_fd("", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static int	init_var(int *status, char ***env, char **envp)
{
	*env = dup_array(envp);
	if (!(*env))
		return (0);
	*status = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	return (1);
}

static void	handle_exit(t_ast *ast, int status, char **input, char ***env)
{
	if (!ast)
		return ;
	if (!ast->args)
		return ;
	if (!ast->args[0])
		return ;
	if (ast->type == NODE_CMD
		&& !ft_strncmp(ast->args[0], "exit", ft_strlen(ast->args[0])))
	{
		free(*input);
		free_array(*env);
		ft_putendl_fd("exit", 1);
		status = ft_exit(ast->args, status);
		ast_free(ast);
		exit(status);
	}
}

static int	minishell(int *status, char **input, char ***env)
{
	t_token	*tokens;
	t_ast	*ast;
	t_data	data;

	data.env = *env;
	data.status = status;
	data.input = input;
	tokens = lexer(input, status, *env);
	lexer_handle_other_lines(tokens, &data);
	if (tokens)
	{
		ast = parser(tokens, status, *env);
		free_token(tokens);
		data.ast = ast;
		browse_ast_for_heredoc(data.ast, &data);
		if (ast && g_sig_status == 4)
			return (ast_free(ast), 1);
		if (ast && g_sig_status != 4)
		{
			handle_exit(ast, *status, input, env);
			*status = executor(ast, *status, env, &data);
			ast_free(ast);
		}
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	int		status;
	char	*input;
	char	**env;

	(void)ac;
	(void)av;
	if (!init_var(&status, &env, envp))
		return (error_creating_env());
	while (1)
	{
		g_sig_status = -1;
		input = readline("minishell$ ");
		if (g_sig_status == 0)
			status = 130;
		if (!input)
			return (free_array(env), ft_putendl_fd("exit", 1), 0);
		g_sig_status = 1;
		minishell(&status, &input, &env);
		if (status != 130)
			add_history(input);
		free(input);
	}
	return (free_array(env), ft_putendl_fd("exit", 1), 0);
}
