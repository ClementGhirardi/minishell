/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_and_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:53:13 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/03 13:49:48 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell.h"

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*tmp;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		tmp = ft_strdup(s2);
	else if (s1 && !s2)
		tmp = ft_strdup(s1);
	else
		tmp = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (tmp);
}
