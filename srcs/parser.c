/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 15:44:51 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_word(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type == TOKEN_WORD)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

static t_ast	*parse_command(t_token **tokens)
{
	t_ast	*instr;
	char	**args;
	int		count;
	int		i;

	count = count_word(*tokens);
	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		args[i] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		i++;
	}
	args[i] = NULL;
	instr = ast_new_cmd(args);
	if (!instr)
		return (free_array(args), NULL);
	return (instr);
}

static t_ast	*parse_instructions(t_token **tokens, int *status, char **env);

static t_ast	*parse_redirection(t_token **tokens)
{
	t_ast			*instr;
	t_token_type	redir_type;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	instr = ast_new_redir(redir_type, *tokens);
	*tokens = (*tokens)->next;
	return (instr);
}

static t_ast	*parse_instructions(t_token **tokens, int *status, char **env)
{
	t_ast	*instr;
	t_ast	*cmd;

	if (!(*tokens) || (*tokens)->type == TOKEN_PIPE)
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD)
	{
		cmd = parse_command(tokens);
		instr = parse_instructions(tokens, status, env);
		ast_add_end(&instr, cmd);
	}
	else
	{
		instr = parse_redirection(tokens);
		if (!instr)
			return (NULL);
		instr->left = parse_instructions(tokens, status, env);
	}
	return (instr);
}

t_ast	*parser(t_token *tokens, t_data *data)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*new_ast;

	left = parse_instructions(&tokens, data->status, data->env);
	if (!left)
		return (NULL);
	while (tokens && tokens->type == TOKEN_PIPE)
	{
		tokens = tokens->next;
		right = parse_instructions(&tokens, data->status, data->env);
		if (left && !right)
			return (ast_free(left), NULL);
		new_ast = ast_new_pipe(left, right);
		if (!new_ast)
			return (ast_free(left), ast_free(right), NULL);
		left = new_ast;
	}
	free_token(data->tokens);
	data->ast = left;
	browse_ast_for_heredoc(left, data);
	if (left && g_sig_status == 4)
		return (ast_free(left),
			free(*data->input), *data->input = NULL, NULL);
	return (left);
}
