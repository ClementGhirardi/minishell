/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbarth <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 11:38:37 by adbarth           #+#    #+#             */
/*   Updated: 2026/04/17 11:38:39 by adbarth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	error_heredocword(char limiter, char **env)
{
	if (limiter == '\'' || limiter == '"')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
		ft_putchar_fd(limiter, 2);
		ft_putchar_fd('\'', 2);
		ft_putendl_fd("", 2);
		ft_exit(NULL, &env);
	}
	else
	{
		ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
		ft_exit(NULL, &env);
	}
}

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

void	*syntax_error(char *c)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(c, 2);
	ft_putendl_fd("'", 2);
	return (NULL);
}
