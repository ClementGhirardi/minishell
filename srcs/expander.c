/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:41:43 by cghirard          #+#    #+#             */
/*   Updated: 2026/03/02 18:09:37 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_var_char(char c)
{
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

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*tmp;

	if (!s1 || !s2)
		return (NULL);
	tmp = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (tmp);
}

char	*expand_string(char *str, t_quote quote)
{
	int		i;
	char	*result;
	char	*var;

	if (quote == SQUOTE)
		return (str);
	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			var = getenv(extract_var_name(str, &i));
		}
		else
		{
			var = malloc (2 * sizeof(char));
			var[0] = str[i];
			var[1] = '\0';
			i++;
		}
		result = ft_strjoin_and_free(result, var);
	}
	return (result);
}

void	expander(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			node->args[i] = expand_string(node->args[i], node->quotes[i]);
			i++;
		}
	}
	if (node->file)
		node->file = expand_string(node->file, node->quotes[0]);
	expander(node->left);
	expander(node->right);
}
