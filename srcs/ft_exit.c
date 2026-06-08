/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:31:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 16:01:20 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	check_fd(t_ast *ast, int fd)
{
	if (!ast)
		return (0);
	if (ast->fd == fd)
		return (1);
	return (check_fd(ast->left, fd) + check_fd(ast->right, fd));
}

static void	free_and_close(t_data *data, int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO && fd_in != -1 && !check_fd(data->ast, fd_in))
		close(fd_in);
	if (fd_out != STDOUT_FILENO && fd_out != -1 && !check_fd(data->ast, fd_out))
		close(fd_out);
	if (data)
	{
		ast_free(data->ast);
		free_array(data->env);
	}
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
	free(*data->input);
	ft_putendl_fd("exit", 1);
	exit(tmp);
}

// static void	error_num(char *arg, int *status)
// {
// 	ft_putstr_fd("minishell: exit: ", 2);
// 	ft_putstr_fd(arg, 2);
// 	ft_putendl_fd(": numeric argument required", 2);
// 	*status = 2;
// }

// static void	error_too_many_args(int *status)
// {
// 	ft_putendl_fd("minishell: exit: too many arguments", 2);
// 	*status = 1;
// }

// static int	ft_strslen(char **strs)
// {
// 	int	len;

// 	len = 0;
// 	if (!strs)
// 		return (len);
// 	while (strs[len])
// 		len++;
// 	return (len);
// }

// static int	ft_is_str_digit(char *str)
// {
// 	int	i;

// 	if (!str)
// 		return (0);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (!ft_isdigit(str[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// int	ft_exit(char **args, int status)
// {
// 	int	len;

// 	if (!args)
// 		return (status);
// 	len = ft_strslen(args);
// 	if (len >= 2)
// 	{
// 		if (!ft_is_str_digit(args[1]))
// 			error_num(args[1], &status);
// 		else if (len > 2)
// 			error_too_many_args(&status);
// 		else
// 			status = ft_atoi(args[1]);
// 	}
// 	return (status);
// }
