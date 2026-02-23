/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 23:32:56 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/12 14:43:12 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_strclen(char const *s, char c)
{
	size_t	len;

	len = 0;
	while ((s[len] != c) && (s[len]))
		len++;
	return (len);
}

static size_t	ft_size(char const *s, char c)
{
	size_t	siz;
	size_t	i;

	siz = 0;
	i = 0;
	while (s[i])
	{
		while ((s[i] == c) && (s[i]))
			i ++;
		if (s[i])
		{
			i += ft_strclen(&s[i], c);
			siz ++;
		}
	}
	return (siz);
}

static char	*ft_strcdup(char const *s, char c)
{
	char		*newstr;
	size_t		i;
	size_t		len;

	len = ft_strclen(s, c);
	newstr = malloc ((len + 1) * sizeof(char));
	if (!newstr)
		return (NULL);
	i = 0;
	while ((s[i]) && (i < len))
	{
		newstr[i] = s[i];
		i ++;
	}
	newstr[i] = '\0';
	return (newstr);
}

static char	**ft_freepast(char **strs, size_t nb)
{
	size_t	j;

	j = 0;
	while (j < nb)
	{
		free(strs[j]);
		j++;
	}
	free(strs);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char		**strs;
	size_t		siz;
	size_t		nb;
	size_t		i;

	siz = ft_size(s, c);
	strs = malloc ((siz + 1) * sizeof(char *));
	if (!strs)
		return (NULL);
	nb = 0;
	i = 0;
	while (nb < siz)
	{
		while ((s[i] == c) && (s[i]))
			i ++;
		strs[nb] = ft_strcdup(&s[i], c);
		if (!strs[nb])
			return (ft_freepast(strs, nb));
		i += ft_strclen(&s[i], c);
		nb ++;
	}
	strs[nb] = NULL;
	return (strs);
}
