/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:54:39 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/01 17:08:57 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_sig_status = 0;

static int	init_var(int *status, char ***env, char **envp)
{
	*env = dup_array(envp);
	if (!(*env))
		return (0);
	*status = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = event;
	return (1);
}

static t_token	*build_tokens(t_data *data, char **input)
{
	t_token	*tokens;
	t_token	*tmp;

	tokens = lexer(data, input);
	if (!tokens)
		return (NULL);
	lexer_handle_other_lines(tokens, data);
	tmp = tokens;
	tokens = split_bracket(&tokens);
	free_token(tmp);
	return (tokens);
}

static t_data	init_data(char **env, int *status, char **input)
{
	t_data	data;

	data.env = env;
	data.status = status;
	data.input = input;
	data.ast = NULL;
	data.other_lines = NULL;
	data.history = 1;
	return (data);
}

static int	minishell(int *status, char **input, char ***env)
{
	t_token	*tokens;
	t_token	*tmp;
	t_ast	*ast;
	t_data	data;

	data = init_data(*env, status, input);
	tokens = build_tokens(&data, input);
	tmp = tokens;
	if (tokens)
	{
		ast = parser(&tokens, &data);
		free_token(tmp);
		data.ast = ast;
		browse_ast_for_heredoc(data.ast, &data);
		if (ast && g_sig_status == 4)
			return (ast_free(ast), free(*data.input), *data.input = NULL, 1);
		if (ast && g_sig_status != 4)
		{
			if (ast->args && !ft_strcmp(ast->args[0], "exit"))
				return (*status = ft_exit(&data, *env, -1, 1), data.history);
			*status = executor(ast, &data, STDIN_FILENO, STDOUT_FILENO);
			return (*env = data.env, ast_free(ast), data.history);
		}
	}
	return (data.history);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**env;
	int		status;
	int		history;

	(void)ac;
	(void)av;
	env = NULL;
	if (!init_var(&status, &env, envp))
		return (error_creating_env());
	while (1)
	{
		g_sig_status = -1;
		input = readline("minishell$ ");
		if (g_sig_status == 0)
			status = 130;
		if (!input && g_sig_status == -1)
			return (free_array(env), ft_exit(NULL, env, -1, -1), status);
		g_sig_status = 1;
		history = minishell(&status, &input, &env);
		if (history)
			add_history(input);
		free(input);
	}
	return (free_array(env), ft_exit(NULL, env, -1, 1), status);
}
