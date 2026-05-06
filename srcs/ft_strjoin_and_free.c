/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_and_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:53:13 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/01 11:53:31 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_sep(char *s1, char *s2, char c)
{
	char	*result;
	int		i;
	int		j;

	if (!*s1)
		return (ft_strdup(s2));
	result = malloc(sizeof(char)
			+ (ft_safe_strlen(s1) + ft_safe_strlen(s2) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	result[i++] = c;
	j = 0;
	while (s2 && s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	return (result);
}

char	*ft_strjoinsep_free(char *s1, char *s2, char c)
{
	char	*tmp;

	tmp = ft_strjoin_sep(s1, s2, c);
	free(s1);
	free(s2);
	return (tmp);
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*result;
	int		i;

	if (!s1)
		return (ft_strdup(&c));
	i = -1;
	result = malloc(sizeof(char) * (ft_strlen(s1) + 2));
	if (!result)
		return (NULL);
	while (*s1)
		result[++i] = *s1++;
	result[++i] = c;
	result[++i] = '\0';
	return (result);
}

char	*ft_strjoin_char_free(char *s1, char c)
{
	char	*tmp;

	if (!s1 && !c)
		return (NULL);
	if (!s1 && c)
		return (ft_strdup(&c));
	if (s1 && !c)
		return (s1);
	tmp = ft_strjoin_char(s1, c);
	free(s1);
	return (tmp);
}

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*tmp;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		tmp = ft_strdup(s2);
	else if (s1 && !s2)
		tmp = ft_strdup(s1);
	else
		tmp = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (tmp);
}
