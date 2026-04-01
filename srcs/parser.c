/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 10:31:49 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_word(t_token *tokens)
{
	int		count;

	count = 0;
	while (tokens && (tokens->type == TOKEN_WORD
			|| tokens->type == TOKEN_WORD_SQUOTE
			|| tokens->type == TOKEN_WORD_DQUOTE))
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
	t_quote	*quotes;
	int		count;
	int		i;

	count = count_word(*tokens);
	if (count == 0)
		return (NULL);
	args = malloc((count + 1) * sizeof(char *));
	quotes = malloc((count) * sizeof(t_quote));
	i = 0;
	while (*tokens && ((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_WORD_SQUOTE
			|| (*tokens)->type == TOKEN_WORD_DQUOTE))
	{
		args[i] = ft_strdup((*tokens)->value);
		quotes[i++] = (*tokens)->type - TOKEN_WORD;
		*tokens = (*tokens)->next;
	}
	args[i] = NULL;
	instr = ast_new_cmd(args, quotes);
	return (instr);
}

static t_ast	*parse_instructions(t_token **tokens);

static t_ast	*parse_redirection(t_token **tokens)
{
	t_ast			*instr;
	t_token_type	redir_type;
	char			*file;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || !((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_WORD_SQUOTE
			|| (*tokens)->type == TOKEN_WORD_DQUOTE))
		return (NULL);
	file = ft_strdup((*tokens)->value);
	if (!file)
		return (NULL);
	instr = ast_new_redir(redir_type, file, (*tokens)->type);
	*tokens = (*tokens)->next;
	return (instr);
}

static t_ast	*parse_instructions(t_token **tokens)
{
	t_ast	*instr;
	t_ast	*cmd;

	if (!(*tokens) || (*tokens)->type == TOKEN_PIPE)
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD
		|| (*tokens)->type == TOKEN_WORD_SQUOTE
		|| (*tokens)->type == TOKEN_WORD_DQUOTE)
	{
		cmd = parse_command(tokens);
		instr = parse_instructions(tokens);
		ast_add_end(&instr, cmd);
	}
	else
	{
		instr = parse_redirection(tokens);
		instr->left = parse_instructions(tokens);
	}
	return (instr);
}

t_ast	*parse(t_token *tokens)
{
	t_ast	*left;
	t_ast	*right;

	if (!tokens)
		return (NULL);
	left = parse_instructions(&tokens);
	while (tokens && tokens->type == TOKEN_PIPE)
	{
		tokens = tokens->next;
		right = parse_instructions(&tokens);
		left = ast_new_pipe(left, right);
	}
	return (left);
}
