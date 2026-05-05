/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:31:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/04 15:21:51 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_num(char *arg, int *status)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	*status = 2;
}

int	error_too_many_args(int *status)
{
	ft_putendl_fd("minishell: exit: too many arguments", 2);
	*status = 1;
	return (*status);
}

int	ft_strslen(char **strs)
{
	int	len;

	len = 0;
	if (!strs)
		return (len);
	while (strs[len])
		len++;
	return (len);
}

int	ft_is_str_digit(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, char ***env, int status)
{
	int	len;

	ft_putstr_fd("exit\n", 1);
	len = ft_strslen(args);
	if (len >= 2)
	{
		if (!ft_is_str_digit(args[1]))
			error_num(args[1], &status);
		else if (len > 2)
			return (error_too_many_args(&status));
		else
			status = ft_atoi(args[1]);
	}
	rl_clear_history();
	free_array(*env);
	exit(status);
}
