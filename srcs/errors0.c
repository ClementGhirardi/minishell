/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:27:59 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 14:02:56 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	error_creating_env(void)
{
	ft_putendl_fd("minishell: can't create new environment", 2);
	return (1);
}

static int	is_an_env_var(char *path, char **env)
{
	int	i;
	int	j;

	if (!path)
		return (0);
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		if (!ft_strcmp(&env[i][j + 1], path))
			return (1);
		i++;
	}
	return (0);
}

static int	error_exec_cmd_slash(char *arg, int *status, char **env)
{
	if (arg[0] == '/')
	{
		*status = 126;
		if (!existing_path(arg))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd(": No such file or directory", 2);
		}
		else if (is_an_env_var(arg, env))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd(": Is a directory", 2);
		}
		else
			return (0);
		return (1);
	}
	return (0);
}

int	error_exec_cmd(char *arg, int *status, char **env)
{
	DIR	*dir;

	if (error_exec_cmd_slash(arg, status, env))
		return (1);
	if (!ft_strncmp(arg, ".", ft_strlen(arg)))
		return (*status = 2,
			ft_putendl_fd("minishell: .: filename argument required", 2),
			ft_putendl_fd(".: usage: . filename [arguments]", 2), 1);
	if (!ft_strncmp(arg, "./", 2))
	{
		dir = opendir(&arg[2]);
		if (!dir)
			return (0);
		closedir(dir);
		*status = 126;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": Is a directory", 2);
		return (1);
	}
	return (0);
}

int	error_here_doc(int *fd, int nb_line, char *limiter, int status)
{
	if (status == 130)
		return (close(fd[1]), close(fd[0]), -1);
	ft_putstr_fd("\nminishell: warning: here-document at line ", 2);
	ft_putnbr_fd(nb_line, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiter, 2);
	ft_putendl_fd("')", 2);
	return (close(fd[1]), fd[0]);
}

void	error_file(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	if (file)
		ft_putstr_fd(file, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
}

static char	*replace(char *new, char *command)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	if (!new || !command)
		return (NULL);
	new[++j] = '$';
	new[++j] = '\'';
	while (command[++i])
	{
		if (command[i] == '\n')
		{
			new[++j] = '\\';
			new[++j] = 'n';
		}
		else
			new[++j] = command[i];
	}
	new[++j] = '\'';
	new[++j] = '\0';
	return (new);
}

static int	count_occurrences(char *s, char c)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == c)
			i++;
		i++;
	}
	return (i);
}

static char	*newline_replace(char *command)
{
	int		i;
	char	*new;

	if (!command)
		return (NULL);
	i = count_occurrences(command, '\n');
	if (!i)
		return (ft_strdup(command));
	new = malloc(sizeof(char) * (i + 4));
	if (!new)
		return (NULL);
	return (replace(new, command));
}

void	error_command(char *command)
{
	char	*new_command;

	if (command && ft_is_in('\n', command))
		new_command = newline_replace(command);
	else
		new_command = ft_strdup(command);
	ft_putstr_fd("minishell: ", 2);
	if (new_command)
		ft_putstr_fd(new_command, 2);
	ft_putendl_fd(": command not found", 2);
	free(new_command);
}

int	error_open(char *file)
{
	if (!file)
		return (ft_putendl_fd("minishell: : No such file or directory", 2), 1);
	if (file[0] == '$')
		return (1);
	if (access(file, F_OK))
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(file, 2),
			ft_putendl_fd(": No such file or directory", 2), 1);
	if (access(file, X_OK))
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(file, 2),
			ft_putendl_fd(": Permission denied", 2), 1);
	return (ft_putstr_fd("minishell: ", 2),
		ft_putendl_fd(": open failed", 2), 1);
}

// void	error_heredocword(char limiter, int status, char **env)
// {
// 	if (limiter == '\'' || limiter == '"')
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
// 		ft_putchar_fd(limiter, 2);
// 		ft_putchar_fd('\'', 2);
// 		ft_putendl_fd("", 2);
// 		status = 2;
// 		ft_exit(NULL, &env, status, STDIN_FILENO, STDIN_FILENO);
// 	}
// 	else
// 	{
// 		ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
// 		ft_exit(NULL, &env, status, STDIN_FILENO, STDIN_FILENO);
// 	}
// }

void	error_heredoc(int i, char *limiter)
{
	char	*nb;

	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	nb = ft_itoa(i);
	if (nb)
		ft_putstr_fd(nb, 2);
	free(nb);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiter, 2);
	ft_putendl_fd("')", 2);
}

void	*syntax_error(char *str, int *status)
{
	*status = 2;
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("'", 2);
	return (NULL);
}
