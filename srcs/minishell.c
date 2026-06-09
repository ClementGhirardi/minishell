/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 14:06:52 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 16:44:57 by cghirard         ###   ########.fr       */
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

// static void	handle_exit(t_ast *ast, t_data *data)
// {
// 	if (!ast)
// 		return ;
// 	if (ast->type == NODE_PIPE)
// 		return ;
// 	if (ast->type == NODE_CMD)
// 	{
// 		if (ast->args && ast->args[0]
// 			&& !ft_strncmp(ast->args[0], "exit", ft_strlen(ast->args[0])))
// 		{
// 			free(*data->input);
// 			executor(data->ast, data, STDIN_FILENO, STDOUT_FILENO);
// 		}
// 	}
// 	return (handle_exit(ast->left, data), handle_exit(ast->right, data));
// }

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
			*status = executor(ast, &data, STDIN_FILENO, STDOUT_FILENO);
			*env = data.env;
			if (data.ast)
				ast_free(ast);
		}
	}
	return (data.update_history);
}
