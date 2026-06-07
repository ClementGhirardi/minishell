/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:27:31 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 15:42:45 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_one_line(char *lines, int *i)
{
	int	start;

	start = *i;
	while (lines[*i])
	{
		if (lines[*i] == '\n')
			return ((*i)++, ft_substr(lines, start, *i - start));
		(*i)++;
	}
	return (NULL);
}

char	*expand_only_var(char *str, int status, char **env)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			tmp = extract_var_name(str, &i, status, env);
		else
			tmp = ft_substr(str, i++, 1);
		result = ft_strjoin_and_free(result, tmp);
	}
	return (free(str), result);
}

void	add_history_noendl(char *str)
{
	char	*tmp;

	if (!str)
		return ;
	if (ft_strlen(str) < 2)
		return ;
	tmp = ft_substr(str, 0, ft_strlen(str) - 1);
	add_history(tmp);
	free(tmp);
}
