/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:23:02 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 14:23:33 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_heredocword(char limiter, int status, char **env)
{
	if (limiter == '\'' || limiter == '"')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
		ft_putchar_fd(limiter, 2);
		ft_putchar_fd('\'', 2);
		ft_putendl_fd("", 2);
		status = 2;
		ft_exit(NULL, &env, status);
	}
	else
	{
		ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
		ft_exit(NULL, &env, status);
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

void	*syntax_error(char *str, int *status)
{
	*status = 2;
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("'", 2);
	return (NULL);
}
