/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:48:22 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/05 16:05:46 by cghirard         ###   ########.fr       */
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
# include <sys/stat.h>

typedef enum e_token_type
{
	TOKEN_WORD,
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

typedef struct s_data
{
	char	**env;
	int		*status;
	char	**input;
	int		end_with_pipe;
	char	*other_lines;
	t_token	*tokens;
	t_ast	*ast;
	int		update_history;
}	t_data;

extern volatile sig_atomic_t	g_sig_status;

int			minishell(int *status, char **input, char ***env);

/* LEXER */
t_token		*lexer(char **input, t_data *data);

t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **tokens, t_token *new);
void		free_token(t_token *tokens);

int			end_with_pipe(char *input);
void		handle_end_with_pipe(char **input, t_data *data);
void		here_doc_word(char limiter, t_data *data);

int			handle_quotes(char **input);
void		handle_pipe(t_token **tokens, int *i);
void		handle_redir(char *input, t_token **tokens, int *i, int dir);
void		create_word(char *input, t_token **tokens, int i, int start);
void		handle_word(char *input, t_token **tokens, int *i);

int			syntax_analyzer(t_token *tokens, int *status);

void		create_other_lines(char *input, t_token **tokens,
				int *i, int start);
void		lexer_handle_other_lines(t_token *tokens, t_data *data);

/* PARSER */
t_ast		*parser(t_token *tokens, t_data *data);

t_ast		*ast_new_cmd(char **args);
t_ast		*ast_new_redir(t_token_type r_type, t_token *tokens);
t_ast		*ast_new_pipe(t_ast *left, t_ast *right);
void		ast_add_end(t_ast **ast, t_ast *new);
void		ast_free(t_ast *ast);

t_ast		*parse_instructions(t_token **tokens, int *status, char **env);
void		browse_ast_for_heredoc(t_ast *ast, t_data *data);
int			here_doc(char *limiter, t_data *data, t_ast *current);
char		*get_one_line(char *lines, int *i);
char		*expand_only_var(char *str, int status, char **env);
void		add_history_noendl(char *str);

/* EXPANDER*/
void		expander(t_ast *node, int status, char **env);

char		*expand_string(char *str, int status, char **env);
char		**remove_empty_var(char **args, int status, char **env);
char		*extract_var_name(char *str, int *i, int status, char **env);
char		*extract_quotevar_name(char *str, int *i, int status, char **env);

/* EXECUTOR */
int			executor(t_ast *ast, t_data *data, int fd_in, int fd_out);

int			execute_pipe(t_ast *node, t_data *data, int fd_in, int fd_out);
int			execute_redir(t_ast *node, t_data *data, int fd_in, int fd_out);
int			execute_cmd(t_ast *node, t_data *data, int fd_in, int fd_out);

/* BUILTINS */
int			is_builtin(char *cmd);
int			run_builtin(t_ast *node, t_data *data, int fd_in, int fd_out);

int			ft_echo(char **args, int fd_out);
int			ft_setenv(char ***env, const char *name, const char *value);
int			ft_cd(char **args, char ***env, int fd_out);
int			ft_pwd(int fd_out);
int			ft_export(char **args, char ***env, int status, int fd_out);
int			ft_unset(char **args, char ***env);
int			ft_env(char ***env, int fd_out);
int			ft_exit(t_data *data, t_ast *node, int fd_in, int fd_out);
// int			ft_exit(char **args, int status);

/* ERROR */
void		error_heredoc(int i, char *limiter);
int			error_open(char *file);
void		error_command(char *command);
void		error_file(char *file);
int			error_here_doc(int *fd, int nb_line, char *limiter, int status);
int			err_exe_cmd(char *arg, int *status, char **env);
// int			error_exec_cmd(char *arg, int *status, char **env);
int			error_creating_env(void);
void		*error_syntax(char *str, int *status);

int			error_too_many_args(int *status);
void		error_num(char *arg, int *status);

/* UTILS */
int			ft_is_in(char c, char *str);
char		*ft_strjoin_and_free(char *s1, char *s2);
int			get_buffer(char **buffer, int *nb_line, t_data *data, int *fds);
int			get_status(int status);
char		*ft_getenv(char **env, const char *name);
char		**ft_realloc(void **ptr, size_t size);
int			ft_strcmp(char *s1, char *s2);
void		free_data(t_data *data);
char		*get_path(char *cmd, char **envp);
int			existing_path(char *cmd);

void		free_array(char **array);
char		**dup_array(char **array);
char		**sort_array(char **array);

#endif