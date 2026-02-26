/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:41:43 by cghirard          #+#    #+#             */
/*   Updated: 2026/02/26 14:13:02 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_var_char(char c)
{
	char	*valid_chars;

	if (('a' <= c && c <= 'z')
		|| ('A' <= c && c <= 'Z')
		|| ('0' <= c && c <= '9')
		|| (c == '_'))
		return (1);
	return (0);
}

char	*extract_var_name(char *str, int *i)
{
	int	start;

	start = *i;
	while (is_var_char(str[*i]))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*expand_string(char *str, t_quote quote, char **envp)
{
	int		i;
	char	*result;
	char	*var;
	char	*tmp;

	if (quote == SQUOTE)
		return (str);
	i = 0;
	*result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			var = extract_var_name(str, &i);
			tmp = ft_strjoin(result, getenv(var));
		}
		else
		{
			var = malloc (2 * sizeof(char));
			var[0] = result[i];
			var[1] = '\0';
			tmp = ft_strjoin(result, var);
			i++;
		}
		free(result);
		free(var);
		result = tmp;
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
