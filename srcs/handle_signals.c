/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 11:04:42 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/24 11:04:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigint_handler(int sig)
{
	(void) sig;
	if (g_sig_status == -1)
		g_sig_status = 0;
	if (g_sig_status == 1)
	{
		g_sig_status = 2;
		ft_putchar_fd('\n', 1);
		return ;
	}
	if (g_sig_status == 3)
	{
		g_sig_status = 4;
		rl_replace_line("", 0);
		rl_done = 1;
		return ;
	}
	ft_putendl_fd("", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	event(void)
{
	return (0);
}
