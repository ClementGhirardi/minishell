/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 14:06:52 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 15:27:58 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

// static void	handle_exit(t_ast *ast, int status, char **input, char ***env)
// {
// 	if (!ast)
// 		return ;
// 	if (!ast->args)
// 		return ;
// 	if (!ast->args[0])
// 		return ;
// 	if (ast->type == NODE_CMD
// 		&& !ft_strncmp(ast->args[0], "exit", ft_strlen(ast->args[0])))
// 	{
// 		if (ast->args[1])
// 			status = ft_atoi(ast->args[1]);
// 		free(*input);
// 		free_array(*env);
// 		ast_free(ast);
// 		*env = NULL;
// 		*input = NULL;
// 		ast = NULL;
// 		ft_putendl_fd("exit", 1);
// 		exit(status);
// 	}
// }

static void	handle_exit(t_ast *ast, t_data *data)
{
	if (!ast)
		return ;
	if (ast->type == NODE_PIPE)
		return ;
	if (ast->type == NODE_CMD)
	{
		if (ast->args && ast->args[0]
			&& !ft_strncmp(ast->args[0], "exit", ft_strlen(ast->args[0])))
			{
				free(*data->input);
				executor(data->ast, data, STDIN_FILENO, STDOUT_FILENO);
				// ft_exit(data, ast, STDIN_FILENO, STDOUT_FILENO);
			}
	}
	return (handle_exit(ast->left, data), handle_exit(ast->right, data));
}

int	minishell(int *status, char **input, char ***env)
{
	t_token	*tokens;
	t_ast	*ast;
	t_data	data;

	init_data(status, input, *env, &data);
	tokens = lexer(input, &data);
	if (tokens)
	{
		ast = parser(tokens, &data);
		if (!ast)
			return (0);
		if (ast && g_sig_status != 4)
		{
			handle_exit(ast, &data);
			// if (ast->args && !ft_strcmp(ast->args[0], "exit"))
			// 	return (ft_putendl_fd("exit", 1),
			// 		*status = ft_exit(&data, (&data)->ast, -1, 1),
			// 		data.update_history);
			*status = executor(ast, &data, STDIN_FILENO, STDOUT_FILENO);
			*env = data.env;
			ast_free(ast);
		}
	}
	return (data.update_history);
}
