/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/14 13:51:34 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_word(t_token *tokens)
{
	int		count;

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
	if (count == 0)
		return (NULL);
	args = malloc((count + 1) * sizeof(char *));
	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		args[i] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		i++;
	}
	args[i] = NULL;
	instr = ast_new_cmd(args);
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
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		return (ast_new_redir(redir_type, NULL));
	else
		file = ft_strdup((*tokens)->value);
	instr = ast_new_redir(redir_type, file);
	*tokens = (*tokens)->next;
	return (instr);
}

static t_ast	*parse_instructions(t_token **tokens)
{
	t_ast	*instr;
	t_ast	*cmd;

	if (!(*tokens) || (*tokens)->type == TOKEN_PIPE)
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD)
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
	if (!left)
		return (ft_putstr_fd("minishell: syntax error ", 2),
			ft_putendl_fd("near unexpected token `|'", 2), NULL);
	while (tokens && tokens->type == TOKEN_PIPE)
	{
		tokens = tokens->next;
		right = parse_instructions(&tokens);
		if (left && !right)
			return (ft_putstr_fd("minishell: syntax error ", 2),
				ft_putendl_fd("near unexpected token `|'", 2), NULL);
		left = ast_new_pipe(left, right);
	}
	return (left);
}
