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

char	*expand_string(char *str, char **env);

int	isnumber(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_ast **ast, char **args, char ***env)
{
	char	*tmp;

	tmp = NULL;
	ft_printf("exit\n");
	if (args && args[0] && args[1])
	{
		tmp = ft_strdup(args[1]);
		tmp = expand_string(tmp, *env);
		if (tmp && !*tmp)
			tmp = ft_strjoin_and_free(tmp, ft_strdup("0"));
		if (tmp && !(isnumber(tmp)))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(tmp, 2);
			ft_putendl_fd(": numeric argument required", 2);
			status = 2;
		}
		else if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			status = 1;
			free(tmp);
			return (status);
		}
		else
			status = ft_atoi(tmp); //args[1]
	}
	if (ast)
		ast_free(*ast);
	if (tmp)
		free(tmp);
	rl_clear_history();
	free_array(*env);
	exit(status);
}
