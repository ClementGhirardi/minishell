/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:48:22 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/25 20:17:08 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "stdlib.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_WORD_SQUOTE,
	TOKEN_WORD_DQUOTE,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_APPEND,
	NODE_HEREDOC
}	t_node_type;

typedef enum e_quote
{
	NQUOTE,
	SQUOTE,
	DQUOTE
}	t_quote;

typedef struct s_ast
{
	t_node_type		type;
	char			**args;
	char			*file;
	t_quote			*quotes;

	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **list, t_token *new);

t_token		*lexer(char *input);

t_ast		*ast_new_node(t_node_type type);
t_ast		*ast_new_cmd(char **args, t_quote *quotes);
t_ast		*ast_new_redir(t_node_type type, char *file,
				t_token_type token_type, t_ast *left);
t_ast		*ast_new_pipe(t_ast *left, t_ast *right);
t_node_type	token_to_node(t_token_type type);

t_ast		*parse(t_token *tokens);

#endif