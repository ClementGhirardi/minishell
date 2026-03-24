/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/24 12:49:04 by cghirard         ###   ########.fr       */
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
	while (tmp && tmp->type == TOKEN_WORD)
	{
		count++;
		tmp = tmp->next;
	}
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
		if (!*tokens || (*tokens)->type != TOKEN_WORD)
			return (NULL);
		type = token_to_node(tmp->type);
		cmd = ast_new_redir(type, (*tokens)->value, cmd);
		*tokens = (*tokens)->next;
		cmd->left = parse_command(tokens);
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
	t_ast	*sub_tree;
	t_token	*tmp;

	left = parse_command(tokens);
	/* Il reste a faire en sorte que | soit prioritaire sur && et || */
	while ((*tokens && (*tokens)->type == TOKEN_PIPE)
		|| (*tokens && (*tokens)->type == TOKEN_AND)
		|| (*tokens && (*tokens)->type == TOKEN_OR))
	{
		if ((*tokens)->bracket)
		{
			tmp = *tokens;
			tmp = tmp->bracket;
			sub_tree = parse_pipeline(&tmp);
			left = ast_new_pipe(left, sub_tree, (*tokens)->type);
			*tokens = (*tokens)->next;
		}
		else
		{
			*tokens = (*tokens)->next;
			right = parse_command(tokens);
			left = ast_new_pipe(left, right, (*tokens)->type);
		}
	}
	return (left);
}

t_ast	*parse(t_token *tokens)
{
	return (parse_pipeline(&tokens));
}
