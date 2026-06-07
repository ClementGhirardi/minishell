/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 14:06:52 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/05 14:33:05 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token	*build_tokens(t_data *data, char **input)
{
	t_token	*tokens;
	t_token	*tmp;

	tokens = lexer(input, data);
	if (!tokens)
		return (NULL);
	tmp = tokens;
	tokens = split_bracket(&tokens);
	free_token(tmp);
	return (tokens);
}

static void	init_data(int *status, char **input, char **env, t_data *data)
{
	data->env = env;
	data->status = status;
	data->input = input;
	data->tokens = NULL;
	data->ast = NULL;
	data->other_lines = NULL;
	data->update_history = 1;
	data->end_with_pipe = 0;
}

int	minishell(int *status, char **input, char ***env)
{
	t_token	*tokens;
	t_token	*tmp;
	t_ast	*ast;
	t_data	data;

	init_data(status, input, *env, &data);
	tokens = build_tokens(&data, input);
	tmp = tokens;
	if (tokens)
	{
		ast = parser(&tokens, &data);
		free_token(tmp);
		if (!ast)
			return (0);
		if (ast && g_sig_status != 4)
		{
			if (ast->args && !ft_strcmp(ast->args[0], "exit"))
				return (ft_putendl_fd("exit", 1),
					*status = ft_exit(&data, (&data)->ast, -1, 1),
					data.update_history);
			*status = executor(ast, &data, STDIN_FILENO, STDOUT_FILENO);
			*env = data.env;
			ast_free(ast);
		}
	}
	return (data.update_history);
}
