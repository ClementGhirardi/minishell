/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:27:31 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/27 14:27:50 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*remove_limiters_quotes(char *file)
{
	char	*new_filename;
	int		i;
	char	quote;

	if (!file)
		return (NULL);
	i = 1;
	if (file[0] == '\'' || file[0] == '"')
	{
		quote = file[0];
		while (file[i] && file[i] != quote)
			i++;
		if (i == 1)
			return (free(file), ft_strdup(""));
	}
	else
	{
		new_filename = ft_strdup(file);
		free(file);
		return (new_filename);
	}
	new_filename = ft_substr(file, 1, i - 1);
	return (free(file), new_filename);
}

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
