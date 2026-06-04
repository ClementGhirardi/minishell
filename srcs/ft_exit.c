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
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	free_and_close(t_data *data, char **env, int fd_in, int fd_out)
{
	if (data)
		ast_free(data->ast);
	if (fd_in != STDIN_FILENO && fd_in != -1)
		close(fd_in);
	if (fd_out != STDOUT_FILENO && fd_out != -1)
		close(fd_out);
	free_array(env);
}

static int	check_exit_args(t_data *data, int *too_many)
{
	int	len;
	int	tmp;

	len = 0;
	tmp = 0;
	ft_putendl_fd("exit", 1);
	if (data && data->ast)
		len = ft_strslen(data->ast->args);
	if (data && data->ast && data->ast->args && len >= 2)
	{
		if (!ft_is_str_digit(data->ast->args[1]))
			error_num(data->ast->args[1], &tmp);
		else if (len > 2)
			return (*too_many = 1, error_too_many_args(&tmp));
		else
			tmp = ft_atoi(data->ast->args[1]);
	}
	return (tmp);
}

int	ft_exit(t_data *data, char **env, int fd_in, int fd_out)
{
	int	tmp;
	int	too_many;

	too_many = 0;
	if (!(data && data->ast && data->ast->type == NODE_CMD
			&& data->ast->args && !ft_strcmp(data->ast->args[0], "exit")))
		return (-1);
	tmp = check_exit_args(data, &too_many);
	if (too_many)
		return (ast_free(data->ast), data->ast = NULL, 1);
	if (wait(NULL) != -1)
		return (free_and_close(data, env, fd_in, fd_out), tmp);
	if (wait(NULL) != -1)
		return (free_and_close(data, env, fd_in, fd_out), tmp);
	free_and_close(data, env, fd_in, fd_out);
	rl_clear_history();
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
