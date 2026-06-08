/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_instructions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 11:36:24 by adbarth           #+#    #+#             */
/*   Updated: 2026/06/08 13:57:41 by cghirard         ###   ########.fr       */
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

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*instr;
	char	**args;
	int		count;
	int		i;

	count = count_word(*tokens);
	if (count == 0)
		return (NULL);
	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		args[i] = ft_strdup((*tokens)->value);
		if (!args[i])
			return (NULL);
		*tokens = (*tokens)->next;
		i++;
	}
	args[i] = NULL;
	instr = ast_new_cmd(args);
	return (instr);
}

t_ast	*parse_redirection(t_token **tokens)
{
	t_ast			*instr;
	t_token_type	redir_type;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		return (ast_new_redir(redir_type, *tokens));
	instr = ast_new_redir(redir_type, *tokens);
	*tokens = (*tokens)->next;
	return (instr);
}

int	check_validity(t_token **tokens)
{
	if (!tokens || !(*tokens) || (*tokens)->type == TOKEN_PIPE
		|| (*tokens)->type == TOKEN_OR || (*tokens)->type == TOKEN_AND)
		return (0);
	return (1);
}

t_ast	*parse_instructions(t_token **tokens, t_data *data)
{
	t_ast	*instr;
	t_ast	*cmd;
	t_token	*tmp;

	if (!check_validity(tokens))
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD && (*tokens)->bracket)
	{
		tmp = (*tokens)->bracket;
		*tokens = (*tokens)->next;
		instr = parser(&tmp, data, NULL);
	}
	else if ((*tokens)->type == TOKEN_WORD)
	{
		cmd = parse_command(tokens);
		instr = parse_instructions(tokens, data);
		ast_add_end(&instr, cmd);
	}
	else
	{
		instr = parse_redirection(tokens);
		if (g_sig_status != 4)
			instr->left = parse_instructions(tokens, data);
	}
	return (instr);
}
