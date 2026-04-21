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

void	error_heredocword(char limiter, char **env, int status)
{
	if (limiter == '\'' || limiter == '"')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
		ft_putchar_fd(limiter, 2);
		ft_putchar_fd('\'', 2);
		ft_putendl_fd("", 2);
		ft_exit(&env, status);
	}
	ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
	ft_exit(&env, status);
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
