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
# include <dirent.h>

extern volatile sig_atomic_t	g_sig_status;

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
	int 	*status;
}	t_infos;

typedef struct s_data
{
	t_token	**tokens;
	char	**input;
	char	*limiter;
}	t_data;

typedef struct s_var
{
	t_token	*current;
	t_token	*previous;

	char	*value;
	char	*new_value;

	int		len;
	char	*line;
}	t_var;

char		*ft_strjoin_and_free(char *s1, char *s2);
char		*ft_strjoin_char_free(char *s1, char c);
char		*ft_strjoinsep_free(char *s1, char *s2, char c);

void		*ft_realloc(void *ptr, size_t size);
char		*ft_strjoin_sep_realloc(char **array, char c);
char		*expand_string_heredoc(char *str, char **env);
void		*ctrld_heredoc(char **history, char *limiter,
				int first_line, int j);

char		*here_doc_word(char limiter, int *status, char **env);
int			ft_strcmp(char *s1, char *s2);
size_t		ft_safe_strlen(char *s);
int			ft_is_in(char c, char *str);

t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **list, t_token *new);
void		free_token(t_token *tokens);

t_token		*lexer(char **input, int *status, char **env);
t_token		*lexer2(char **input, int *status, char **env);
//void		handle_quotes(char **input, int *status, char **env);
t_token		*handle_last_pipe_op(char *input, t_token *tokens, int *status, char **env);
int			handle_quotes(char **input);

void		handle_pipe(char *input, t_token **tokens, int *i);
void		handle_and(char *input, t_token **tokens, int *i);
void		handle_redir(char *input, t_token **tokens, int *i, int dir);

void		*syntax_error(char *str, int *status);

int			syntax_analyzer(t_token *tokens, int *status);

t_token		*split_bracket(t_token **tokens);
void		ft_tokadd_back(t_token **lst, t_token *new);


t_ast		*ast_new_cmd(char **args);
t_ast		*ast_new_redir(t_token_type r_type, int *status, char **env, t_data *data);
t_ast		*ast_new_pipe(t_ast *left, t_ast *right);
t_ast		*ast_new_operator(t_ast *left, t_ast *right, t_token_type type);
void		ast_add_end(t_ast **ast, t_ast *new);
void		ast_free(t_ast *ast);
t_ast		*ast_new_pipe_op(t_ast *left, t_ast *right, t_token_type type);

t_ast		*create_redir_node(int *status, char **env, t_data *data);

char		*ft_gethole_fd(int fd);

t_ast		*parse(t_token **tokens, int *status, char **env, char **input);
t_ast		*parse_command(t_token **tokens);
t_ast		*parse_redirection(t_token **tokens, t_infos *infos);

char		*ft_getenv(char **env, const char *name);

char		*extract_var_name(char *str, int *i, int status, char **env);
void		expander(t_ast *node, int status, char **env);
char		*expand_string(char *str, int status, char **env);
char		*expand_dollar_in_filename(char *file, char **env);
int			dollar_finder(char *file);

char		*quotes_cleaner(char *str);
char		*skip_empty_first_quotes(char *str);
char		**filter_args(char **args, char **env);
int			existing_var(char *var, char **env);
char		*expand_var(char *str, char **env);
char		*skip_empty_quotes(char *str);


char		**filter_and_dup_array(char **array, char **env);
char		**alloc_array(char **array, char **env);
int			is_valid_variable(char *arg, char **env);

int			here_doc(t_data *data, int *status, char **env);
char		*here_doc_pipe_op(char *input_begining, int *status, char **env);

int			idx_to_next_line(char *str);

int			get_buffer(char **buffer, int *nb_line, int *status, char **env);

char		*get_path(char *cmd, char **envp);

int			get_status(int status);

void		free_array(char **array);
char		**dup_array(char **array);
char		**sort_array(char **array);

int			ft_echo(char **args);

int			ft_setenv(char ***env, const char *name, const char *value);

int			ft_cd(char **args, char ***env);

int			ft_pwd(void);

int			ft_export(char **args, char ***env, int status);

int			ft_unset(char **args, char ***env);

int			ft_env(char ***env);

//int			ft_exit(t_ast **ast, char ***env, int status);
int			ft_exit(char **args, char ***env, int status);

int			is_builtin(char *cmd);
int			run_builtin(t_ast *ast, char **args, char ***env, int status);

int			executor(t_ast *ast, int status, char ***env);
int			execute_operator(t_ast *ast, int status, char ***env);
int			execute_redir(t_ast *node, int status, char ***env);
int			execute_pipe(t_ast *node, int status, char ***env);
int			execute_cmd(t_ast *node, int status, char ***env);

void		error_heredoc(int i, char *limiter);
void		error_heredocword(char limiter, int status, char **env);
int			error_open(char *file);
void		error_command(char *command);
void		error_file(char *file);
int			error_here_doc(int *fd, int nb_line, char *limiter, int status);
int			error_exec_cmd(char *arg, int *status, char **env);
int			error_exec_cmd(char *arg, int *status, char **env);

#endif