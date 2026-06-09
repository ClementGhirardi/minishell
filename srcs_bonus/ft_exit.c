/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:31:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 18:07:01 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell.h"

int	ft_is_str_digit(char *str)
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

int	check_fd(t_ast *ast, int fd)
{
	if (!ast)
		return (0);
	if (ast->fd == fd)
		return (1);
	return (check_fd(ast->left, fd) + check_fd(ast->right, fd));
}

void	free_and_close(t_data *data, int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO && fd_in != -1 && !check_fd(data->ast, fd_in))
		close(fd_in);
	if (fd_out != STDOUT_FILENO && fd_out != -1 && !check_fd(data->ast, fd_out))
		close(fd_out);
	if (data)
	{
		ast_free(data->ast);
		free_array(data->env);
		data->ast = NULL;
		data->env = NULL;
	}
}

int	check_exit_args(t_data *data, t_ast *node, int *too_many, int *not_digit)
{
	int	len;
	int	tmp;

	len = 0;
	tmp = 0;
	(void)(data);
	if (node)
		len = ft_strslen(node->args);
	if (node && node->args && len >= 2)
	{
		if (!ft_is_str_digit(node->args[1]))
		{
			*not_digit = 1;
			tmp = 2;
		}
		else if (len > 2)
			return (*too_many = 1, 1);
		else
			tmp = ft_atoi(node->args[1]);
	}
	return (tmp);
}

int	ft_exit(t_data *data, t_ast *node, int fd_in, int fd_out)
{
	int	too_many;
	int	not_digit;

	too_many = 0;
	not_digit = 0;
	*data->status = check_exit_args(data, node, &too_many, &not_digit);
	if (too_many)
		return (ft_putendl_fd("exit\nminishell: exit: too many arguments", 2),
			ast_free(data->ast), data->ast = NULL, *data->status = 1, 1);
	if (not_digit)
	{
		ft_putendl_fd("exit", 2);
		error_num(node->args[1], data->status);
	}
	if (wait(NULL) != -1)
		return (free_and_close(data, fd_in, fd_out), *data->status);
	if (wait(NULL) != -1)
		return (free_and_close(data, fd_in, fd_out), *data->status);
	free_and_close(data, fd_in, fd_out);
	rl_clear_history();
	if (data && data->input)
		free(*data->input);
	if (!not_digit)
		ft_putendl_fd("exit", 1);
	exit(*data->status);
}
