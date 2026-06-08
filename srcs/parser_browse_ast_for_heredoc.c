/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_browse_ast_for_heredoc.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:48:16 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 17:08:47 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*remove_limiters_quotes(char *file)
{
	char	*new_filename;
	int		i;
	char	quote;

	if (!file)
		return (NULL);
	i = 1;
	if (file[0] == '\'' || file[0] == '"')
	{
		quote = file[0];
		while (file[i] && file[i] != quote)
			i++;
		if (i == 1)
			return (free(file), ft_strdup(""));
	}
	else
	{
		new_filename = ft_strdup(file);
		free(file);
		return (new_filename);
	}
	new_filename = ft_substr(file, 1, i - 1);
	return (free(file), new_filename);
}

void	browse_ast_for_heredoc(t_ast *ast, t_data *data)
{
	if (!ast)
		return ;
	if (ast->type == NODE_HEREDOC && ast->fd == -1)
	{
		ast->file = remove_limiters_quotes(ast->file);
		if (!ast->file)
			return ;
		ast->fd = here_doc(ast->file, data, ast);
		if (ast->fd == -1)
			return ;
	}
	browse_ast_for_heredoc(ast->left, data);
	browse_ast_for_heredoc(ast->right, data);
}

t_ast	*parse_and_browse(t_token **tokens, t_data *data)
{
	t_token	*tmp_token;
	t_ast	*tmp_ast;

	data->ast = parser(tokens, data, NULL);
	free_token(data->tokens);
	browse_ast_for_heredoc(data->ast, data);
	if (data->ast && g_sig_status == 4)
		return (ast_free(data->ast), NULL);
	while ((data->ast->type == NODE_PIPE || data->ast->type == NODE_OR
			|| data->ast->type == NODE_AND) && !data->ast->right
		&& g_sig_status != 4)
	{
		here_doc_word('\n', data);
		tmp_token = build_tokens(data, data->input);
		data->tokens = tmp_token;
		if (!data->tokens)
			return (ast_free(data->ast), NULL);
		tmp_ast = parser(&data->tokens, data, data->ast);
		data->ast = tmp_ast;
		free_token(tmp_token);
		browse_ast_for_heredoc(data->ast, data);
		if (data->ast && g_sig_status == 4)
			return (ast_free(data->ast), NULL);
	}
	return (data->ast);
}
