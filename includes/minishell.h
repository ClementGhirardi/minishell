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

//extern int	status;
extern volatile sig_atomic_t	status;

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

typedef struct s_infos
{
	char	**env;
	char	**input;
}	t_infos;

char		*ft_strjoin_and_free(char *s1, char *s2);
char		*ft_strjoin_char_free(char *s1, char c);
char		*ft_strjoinsep_free(char *s1, char *s2, char c);

char		*here_doc_word(char limiter, char **env);
int			ft_strcmp(char *s1, char *s2);
size_t		ft_safe_strlen(char *s);

t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **list, t_token *new);
void		free_token(t_token *tokens);

t_token		*lexer(char **input, char **env);
t_token		*lexer2(char **input, char **env);
void		handle_quotes(char **input, char **env);
t_token		*handle_last_pipe_op(t_token *tokens, char **env);
void		handle_last_and(char **input, char **env);

void		handle_pipe(char *input, t_token **tokens, int *i);
void		handle_and(char *input, t_token **tokens, int *i);
void		handle_redir(char *input, t_token **tokens, int *i, int dir);

void		*syntax_error(char *c);

int			syntax_analyzer(t_token *tokens);

t_token		*split_bracket(t_token **tokens);
void		ft_tokadd_back(t_token **lst, t_token *new);


t_ast		*ast_new_cmd(char **args);
t_ast		*ast_new_redir(t_token_type r_type, char *file, char **env);
t_ast		*ast_new_pipe(t_ast *left, t_ast *right);
t_ast		*ast_new_operator(t_ast *left, t_ast *right, t_token_type type);
void		ast_add_end(t_ast **ast, t_ast *new);
void		ast_free(t_ast *ast);
t_ast		*ast_new_pipe_op(t_ast *left, t_ast *right, t_token_type type);

char		*ft_gethole_fd(int fd);

t_ast		*parse(t_token **tokens, char **env, char **input);
t_ast		*parse_command(t_token **tokens);
t_ast		*parse_redirection(t_token **tokens, t_infos *infos);

char		*ft_getenv(char **env, const char *name);

char		*extract_var_name(char *str, int *i, char **env);
void		expander(t_ast *node, char **env);
char		*expand_string(char *str, char **env);
char		*expand_dollar_in_filename(char *file, char **env);
int			dollar_finder(char *file);

char		**filter_and_dup_array(char **array, char **env);
int			is_valid_variable(char *arg, char **env);

int			here_doc(char *limiter, char **env);
char		*here_doc_pipe_op(char **env);

char		*get_path(char *cmd, char **envp);

int			get_status(void);

void		free_array(char **array);
char		**dup_array(char **array);
char		**sort_array(char **array);

int			ft_echo(char **args);

int			ft_setenv(char ***env, const char *name, const char *value);

int			ft_cd(char **args, char ***env);

int			ft_pwd(void);

int			ft_export(char **args, char ***env);

int			ft_unset(char **args, char ***env);

int			ft_env(char ***env);

int			ft_exit(t_ast **ast, char ***env);
int			is_builtin(char *cmd);
int			run_builtin(t_ast *ast, char **args, char ***env);

int			executor(t_ast *ast, char ***env);
int			execute_operator(t_ast *ast, char ***env);
int			execute_redir(t_ast *node, char ***env);
int			execute_pipe(t_ast *node, char ***env);
int			execute_cmd(t_ast *node, char ***env);

void		error_heredoc(int i, char *limiter);
void		error_heredocword(char limiter, char **env);
int			error_open(char *file);

#endif