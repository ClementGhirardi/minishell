/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:31:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/03 13:49:23 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_strslen(char **strs)
{
	int	len;

	len = 0;
	if (!strs)
		return (len);
	while (strs[len])
		len++;
	return (len);
}

static int	ft_is_str_digit(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	if (i == 0)
		return (0);
	return (1);
}

static void	free_and_close(t_data *data, int fd_in, int fd_out)
{
	if (data)
	{
		ast_free(data->ast);
		free_array(data->env);
	}
	if (fd_in != STDIN_FILENO && fd_in != -1)
		close(fd_in);
	if (fd_out != STDOUT_FILENO && fd_out != -1)
		close(fd_out);
}

static int	check_exit_args(t_data *data, t_ast *node, int *too_many)
{
	int	len;
	int	tmp;

	len = 0;
	tmp = 0;
	if (node && data)
		expander(node, *data->status, data->env);
	if (node)
		len = ft_strslen(node->args);
	if (node && node->args && len >= 2)
	{
		if (!ft_is_str_digit(node->args[1]))
			error_num(node->args[1], &tmp);
		else if (len > 2)
			return (*too_many = 1, error_too_many_args(&tmp));
		else
			tmp = ft_atoi(node->args[1]);
	}
	return (tmp);
}

int	ft_exit(t_data *data, t_ast *node, int fd_in, int fd_out)
{
	int	tmp;
	int	too_many;

	too_many = 0;
	tmp = check_exit_args(data, node, &too_many);
	if (too_many)
		return (ast_free(data->ast), data->ast = NULL, 1);
	if (wait(NULL) != -1)
		return (free_and_close(data, fd_in, fd_out), tmp);
	if (wait(NULL) != -1)
		return (free_and_close(data, fd_in, fd_out), tmp);
	free_and_close(data, fd_in, fd_out);
	rl_clear_history();
	if (data)
		*data->status = tmp;
	exit(tmp);
}
