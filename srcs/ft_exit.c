/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:31:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 14:36:39 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(t_ast **ast, char **args, char ***env)
{
	ft_printf("exit\n");
	if (args && args[0] && args[1])
	{
		if (!args[2])
			status = ft_atoi(args[1]);
		else
		{
			ft_putendl_fd("minishell: exit: too many arguments\n", 2);
			status = 1;
			if (ast)
				ast_free(*ast);
			return (status);
		}
	}
	if (ast)
		ast_free(*ast);
	rl_clear_history();
	free_array(*env);
	exit(status);
}
