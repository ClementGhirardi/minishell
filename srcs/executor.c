/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:53:18 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 14:11:27 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_empty_quotes(char *str)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			if (str[i + 1] == quote)
			{
				i += 2;
				continue ;
			}
		}
		count++;
		i++;
	}
	return (count);
}

char	*ft_strdup_no_empty_quotes(char *str)
{
	char	*result;
	int		len;
	int		i;
	char	quote;

	len = count_empty_quotes(str);
	result = malloc(sizeof(char) * (len + 1));
	i = 0;
	if (!result)
		return (NULL);
	while (str && *str)
	{
		if (*str == '\'' || *str == '"')
		{
			quote = *str;
			if (*(str + 1) == quote)
			{
				str += 2;
				continue ;
			}
		}
		result[i++] = *str++;
	}
	result[i] = '\0';
	return (result);
}

int	executor(t_ast *ast, t_ast *root, int status, char ***env)
{
	if (!ast)
		return (1);
	if (g_sig_status == 2)
		return (130);
	if (ast && ast->type == NODE_CMD)
		return (expander(ast, status, *env),
			execute_cmd(ast, root, status, env));
	else if (ast && ast->type == NODE_PIPE)
		return (execute_pipe(ast, root, status, env));
	else if (ast && (ast->type == NODE_REDIR_IN || ast->type == NODE_REDIR_OUT
			|| ast->type == NODE_APPEND || ast->type == NODE_HEREDOC))
	{
		if (ast->file && ast->type != NODE_HEREDOC)
			expander(ast, status, *env);
		return (execute_redir(ast, root, status, env));
	}
	else if (ast && (ast->type == NODE_AND || ast->type == NODE_OR))
		return (execute_operator(ast, root, status, env));
	else
		return (1);
}
