/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:48:22 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/27 16:51:53 by cghirard         ###   ########.fr       */
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

char		*here_doc_word(char limiter);

t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **list, t_token *new);
void		free_token(t_token *tokens);

void		handle_last_pipe(char **input);

t_token		*lexer(char **input);

t_ast		*ast_new_cmd(char **args);
t_ast		*ast_new_redir(t_token *tmp, int status, char **env, t_data *data);
t_ast		*ast_new_pipe(t_ast *left, t_ast *right);
void		ast_add_end(t_ast **ast, t_ast *new);
void		ast_free(t_ast *ast);

char		*ft_gethole_fd(int fd);

t_ast		*parse(t_token *tokens, int status, char **env, char **input);

char		*ft_getenv(char **env, const char *name);

char		*extract_var_name(char *str, int *i, int status, char **env);
void		expander(t_ast *node, int status, char **env);

int			here_doc(t_data *data, int status, char **env);

char		*get_path(char *cmd, char **envp);

int			get_status(int status);

void		free_array(char **array);
char		**dup_array(char **array);
char		**sort_array(char **array);

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