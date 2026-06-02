/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:48:29 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 16:50:18 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(int fd_out)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (ft_putendl_fd("pwd: error retrieving current directory", fd_out)
			, 1);
	ft_putendl_fd(path, fd_out);
	free(path);
	return (0);
}
