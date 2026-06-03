/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:27:59 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/20 12:08:12 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
