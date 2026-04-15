/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/02 15:44:32 by cghirard         ###   ########.fr       */
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
	return (instr);
}

static t_ast	*parse_instructions(t_token **tokens);

static t_ast	*parse_redirection(t_token **tokens)
{
	t_ast			*instr;
	t_token_type	redir_type;
	char			*file;

	// ft_printf("predir: %s\n", (*tokens)->value);
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
	t_token	*tmp;

	if (!(*tokens) || (*tokens)->type == TOKEN_PIPE
	|| (*tokens)->type == TOKEN_OR || (*tokens)->type == TOKEN_AND)
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD)
	{
		if (!(*tokens)->value[0])
		{
			tmp = (*tokens)->bracket;
			*tokens = (*tokens)->next;
			instr = parse(&tmp);
		}
		else
		{
			cmd = parse_command(tokens);
			instr = parse_instructions(tokens);
			ast_add_end(&instr, cmd);
		}
	}
	else
	{
		instr = parse_redirection(tokens);
		instr->left = parse_instructions(tokens);
	}
	return (instr);
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*brack;
	t_token	*tmp;

	if (!tokens || !*tokens)
		return (NULL);
	left = parse_instructions(tokens);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		brack = NULL;
		if ((*tokens)->bracket)
		{
			tmp = (*tokens)->bracket;
			brack = parse(&tmp);
			left = ast_new_pipe(left, brack);
			*tokens = (*tokens)->next;
		}
		else
		{
			*tokens = (*tokens)->next;
			right = parse_instructions(tokens);
			left = ast_new_pipe(left, right);
		}
	}
	return (left);
}

t_ast	*parse_after_bracket(t_token **tokens, t_ast *left, t_ast *brack)
{
	t_ast			*instr;
	t_token_type	type;

	if (!tokens || !*tokens)
		return (NULL);
	type = (*tokens)->type;
	instr = parse_pipeline(tokens);
	left->right = instr;
	instr->left = brack;
	return (left);
}

t_ast	*parsing_loop(t_token **tokens, t_ast *left, t_ast **brack)
{
	t_ast			*right;
	t_token			*tmp;
	t_token_type	type;

	while ((*tokens && (*tokens)->type == TOKEN_AND)
		|| (*tokens && (*tokens)->type == TOKEN_OR))
	{
		*brack = NULL;
		type = (*tokens)->type;
		if ((*tokens)->bracket)
		{
			tmp = (*tokens)->bracket;
			*brack = parse(&tmp);
			left = ast_new_operator(left, *brack, type);
			*tokens = (*tokens)->next;
		}
		else
		{
			*tokens = (*tokens)->next;
			right = parse_pipeline(tokens);
			left = ast_new_operator(left, right, type);
		}
	}
	return (left);
}

t_ast	*parse(t_token **tokens)
{
	t_ast	*left;
	t_ast	*brack;

	if (!tokens || !*tokens)
		return (NULL);
	brack = NULL;
	left = parse_pipeline(tokens);
	while (*tokens)
	{
		left = parsing_loop(tokens, left, &brack);
		if (*tokens)
			left = parse_after_bracket(tokens, left, brack);
	}
	return (left);
}
