/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:17:53 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/03 13:49:20 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_env(char ***env, int fd_out)
{
	int		i;

	i = 0;
	while ((*env)[i])
	{
		ft_putendl_fd((*env)[i], fd_out);
		i++;
	}
	return (0);
}
