/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/05 13:52:19 by clement-ghi      ###   ########.fr       */
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

static t_ast	*parse_instructions(t_token **tokens, int *status, char **env,
				char **input);

static t_ast	*parse_redirection(t_token **tokens, int *status, char **env,
	char **input)
{
	t_ast			*instr;
	t_token_type	redir_type;
	t_data			data;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!(*tokens))
		return (error_syntax("`newline'", status));
	data.tokens = tokens;
	data.input = input;
	instr = ast_new_redir(redir_type, status, env, &data);
	*tokens = (*tokens)->next;
	return (instr);
}

static t_ast	*parse_instructions(t_token **tokens, int *status, char **env,
	char **input)
{
	t_ast	*instr;
	t_ast	*cmd;

	if (!(*tokens) || (*tokens)->type == TOKEN_PIPE)
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD)
	{
		cmd = parse_command(tokens);
		if (!cmd)
			return (error_syntax("`|'", status));
		instr = parse_instructions(tokens, status, env, input);
		ast_add_end(&instr, cmd);
	}
	else
	{
		instr = parse_redirection(tokens, status, env, input);
		if (!instr)
			return (NULL);
		instr->left = parse_instructions(tokens, status, env, input);
	}
	return (instr);
}

t_ast	*parser(t_token *tokens, int *status, char **env, char **input)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*new_ast;

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_PIPE)
		error_syntax("`|'", status);
	left = parse_instructions(&tokens, status, env, input);
	if (!left)
		return (NULL);
	while (tokens && tokens->type == TOKEN_PIPE)
	{
		tokens = tokens->next;
		right = parse_instructions(&tokens, status, env, input);
		if (left && !right)
			return (ast_free(left), NULL);
		new_ast = ast_new_pipe(left, right);
		if (!new_ast)
			return (ast_free(left), ast_free(right), NULL);
		left = new_ast;
	}
	return (left);
}
