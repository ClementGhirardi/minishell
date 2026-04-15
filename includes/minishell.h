/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:48:22 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/02 14:26:49 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_O_BRACK,
	TOKEN_C_BRACK
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*bracket;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_APPEND,
	NODE_HEREDOC,
	NODE_AND,
	NODE_OR
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	char			**args;
	char			*file;
	int				fd;

	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_instrs
{
	int		fd_in;
	int		fd_out;
	char	**args;
	char	*path;
}	t_instrs;

char		*ft_strjoin_and_free(char *s1, char *s2);
char		*ft_strjoin_char_free(char *s1, char c);

char		*here_doc_word(char limiter);

t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **list, t_token *new);
void		free_token(t_token *tokens);

t_token		*lexer(char **input);
t_token		*lexer2(char **input);
void		*syntax_error(char *c);

t_token		*split_bracket(t_token **tokens);

t_ast		*ast_new_cmd(char **args);
t_ast		*ast_new_redir(t_token_type redir_type, char *file);
t_ast		*ast_new_pipe(t_ast *left, t_ast *right);
t_ast		*ast_new_operator(t_ast *left, t_ast *right, t_token_type type);
void		ast_add_end(t_ast **ast, t_ast *new);
void		ast_free(t_ast *ast);

t_ast		*parse(t_token **tokens);

char		*ft_getenv(char ***env, const char *name);

void		expander(t_ast *node, int status, char ***env);
char		*expand_string(char *str, int status, char ***env);

int			here_doc(char *limiter);

char		*get_path(char *cmd, char **envp);

int			get_status(int status);

void		free_array(char **array);
char		**dup_array(char **array);
char		**sort_array(char **array);

int			ft_echo(char **args, char ***env);

int			ft_setenv(char ***env, const char *name, const char *value);

int			ft_cd(char **args, char ***env);

int			ft_pwd(void);

int			ft_export(char **args, char ***env);

int			ft_unset(char **args, char ***env);

int			ft_env(char ***env);

int			ft_exit(char ***env, int status);

int			is_builtin(char *cmd);
int			run_builtin(char **args, char ***env, int status);

int			executor(t_ast *ast, int status, char ***env);

#endif