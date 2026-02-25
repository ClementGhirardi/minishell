/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:41:43 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/25 23:41:11 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*extract_var_name(char *str, int *i);
char	*join_and_free(char *s1, char *s2);

char	*expand_string(char *str, t_quote quote, char **envp)
{
	int		i;
	char	*result;
	char	*var;
	char	tmp[2];

	i = 0;
	*result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			var = extract_var_name(str, &i);
			result = join_and_free(result, getenv(var));
			free(var);
		}
		else
		{
			tmp[0] = str[i];
			tmp[1] = '\0';
			result = join_and_free(result, tmp);
			i++;
		}
	}
	return (result);
}

void	expand_args(char **args, t_quote *quotes, char **env)
{
	int	i;

	i = 0;
	while (args[i])
	{
		args[i] = expand_string(args[i], quotes[i], env);
		i++;
	}
}

void	expander(t_ast *node, char **env)
{
	if (!node)
		return ;
	if (node->args)
		expand_args(node->args, node->quotes, env);
	if (node->file)
		node->file = expand_string(node->file, node->quotes[0], env);
	expand_ast(node->left, env);
	expand_ast(node->right, env);
}
