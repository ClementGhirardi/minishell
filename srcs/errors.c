/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:27:59 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/28 15:24:47 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	error_creating_env(void)
{
	ft_putendl_fd("minishell: can't create new environment", 2);
	return (0);
}

int	error_here_doc(int *fd, int nb_line, char *limiter)
{
	ft_putstr_fd("\nminishell: warning: here-document at line ", 2);
	ft_putnbr_fd(nb_line, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiter, 2);
	ft_putendl_fd("')", 2);
	close(fd[1]);
	return (fd[0]);
}

void	*error_syntax(char *str, int *status)
{
	*status = 2;
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	ft_putendl_fd(str, 2);
	return (NULL);
}
