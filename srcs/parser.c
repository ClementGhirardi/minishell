/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/25 21:10:24 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**extract_args(t_token **tokens)
{
	char	**args;
	int		count;
	t_token	*tmp;
	int		i;

	count = 0;
	tmp = *tokens;
	while (tmp && (tmp->type == TOKEN_WORD
			|| tmp->type == TOKEN_WORD_SQUOTE
			|| tmp->type == TOKEN_WORD_DQUOTE))
	{
		count++;
		tmp = tmp->next;
	}
	args = malloc((count + 1) * sizeof(char *));
	i = 0;
	while (*tokens && ((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_WORD_SQUOTE
			|| (*tokens)->type == TOKEN_WORD_DQUOTE))
	{
		args[i++] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	args[i] = NULL;
	return (args);
}

t_quote	*extract_quotes(t_token **tokens)
{
	t_quote	*quotes;
	int		count;
	t_token	*tmp;
	int		i;

	count = 0;
	tmp = *tokens;
	while (tmp && (tmp->type == TOKEN_WORD
			|| tmp->type == TOKEN_WORD_SQUOTE
			|| tmp->type == TOKEN_WORD_DQUOTE))
	{
		count++;
		tmp = tmp->next;
	}
	quotes = malloc((count) * sizeof(t_quote));
	i = 0;
	while (*tokens && ((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_WORD_SQUOTE
			|| (*tokens)->type == TOKEN_WORD_DQUOTE))
	{
		quotes[i++] = (*tokens)->type - TOKEN_WORD;
		*tokens = (*tokens)->next;
	}
	return (quotes);
}

t_ast	*parse_command(t_token **tokens);

t_ast	*parse_redirection(t_token **tokens, t_ast *cmd)
{
	t_token		*tmp;
	t_node_type	type;

	while (*tokens
		&& ((*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT
			|| (*tokens)->type == TOKEN_APPEND
			|| (*tokens)->type == TOKEN_HEREDOC))
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		if (!*tokens || !((*tokens)->type == TOKEN_WORD)
			|| ((*tokens)->type == TOKEN_WORD_SQUOTE)
			|| ((*tokens)->type == TOKEN_WORD_DQUOTE))
			return (NULL);
		type = token_to_node(tmp->type);
		cmd = ast_new_redir(type, (*tokens)->value, (*tokens)->type, cmd);
		*tokens = (*tokens)->next;
		cmd->left = parse_command(tokens);
	}
	return (cmd);
}

t_ast	*parse_command(t_token **tokens)
{
	char	**args;
	t_quote	*quotes;
	t_ast	*cmd;

	args = extract_args(tokens);
	quotes = extract_quotes(tokens);
	cmd = ast_new_cmd(args, quotes);
	cmd = parse_redirection(tokens, cmd);
	return (cmd);
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(tokens);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_command(tokens);
		left = ast_new_pipe(left, right);
	}
	return (left);
}

t_ast	*parse(t_token *tokens)
{
	return (parse_pipeline(&tokens));
}
