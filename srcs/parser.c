/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/24 00:48:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#include "minishell.h"
#include "libft.h"

int	count_args(t_token *tokens)
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

char	**extract_args(t_token **tokens)
{
	char	**args;
	int		count;
	int		i;

	count = count_args(*tokens);
	args = malloc((count + 1) * sizeof(char *));
	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		args[i++] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	args[i] = NULL;
	return (args);
}

t_node_type	token_to_node(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (NODE_REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (NODE_REDIR_OUT);
	if (type == TOKEN_APPEND)
		return (NODE_APPEND);
	if (type == TOKEN_HEREDOC)
		return (NODE_HEREDOC);
	return (NODE_CMD);
}

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
		if (!*tokens || (*tokens)->type != TOKEN_WORD)
			return (NULL);
		type = token_to_node(tmp->type);
		cmd = ast_new_redir(type, (*tokens)->value, cmd);
		*tokens = (*tokens)->next;
	}
	return (cmd);
}

t_ast	*parse_command(t_token **tokens)
{
	char	**args;
	t_ast	*cmd;

	args = extract_args(tokens);
	cmd = ast_new_cmd(args);
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
