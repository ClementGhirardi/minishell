/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:31:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/06 16:24:09 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static void	error_num(char *arg, int *status)
// {
// 	ft_putstr_fd("minishell: exit: ", 2);
// 	ft_putstr_fd(arg, 2);
// 	ft_putendl_fd(": numeric argument required", 2);
// 	*status = 2;
// }

// static int	error_too_many_args(int *status)
// {
// 	ft_putendl_fd("minishell: exit: too many arguments", 2);
// 	*status = 1;
// 	return (*status);
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

int	ft_exit(t_ast *ast, char ***env, int status)
{
	//int	len;

	//len = 0;
	// if (ast)
	// 	len = ft_strslen(ast->args);
	// if (len >= 2)
	// {
	// 	if (!ft_is_str_digit(ast->args[1]))
	// 		error_num(ast->args[1], &status);
	// 	else if (len > 2)
	// 		return (error_too_many_args(&status));
	// 	else
	// 		status = ft_atoi(ast->args[1]);
	// }

	// if (wait(NULL) != -1)
	// 	return (ast_free(ast), free_array(*env), status);
	// if (wait(NULL) != -1)
	// 	return (ast_free(ast), free_array(*env), status);
	
	
	// if ((!ast->left && !ast->right)
	// 	|| (!ast->right && ast->left->type != NODE_PIPE)
	// 	|| (!ast->left && ast->right->type != NODE_PIPE)
	// 	|| (ast->left->type != NODE_PIPE
	// 		&& !ast->left && ast->right->type != NODE_PIPE))
	ft_putendl_fd("exit", 1);
	free_array(*env);
	ast_free(ast);
	rl_clear_history();
	exit(status);
}
