/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:44:24 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/12 14:42:26 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_size(long int nb)
{
	size_t	siz;

	siz = 1;
	if (nb < 0)
	{
		siz++;
		nb *= -1;
	}
	while (nb > 9)
	{
		siz++;
		nb /= 10;
	}
	return (siz);
}

static void	ft_fill(char *s, long int nb, size_t siz)
{
	if (nb > 9)
		ft_fill(s, nb / 10, siz - 1);
	s[siz - 1] = nb % 10 + '0';
}

char	*ft_itoa(int n)
{
	char		*s;
	size_t		siz;
	long int	nb;

	nb = (long int) n;
	siz = ft_size(nb);
	s = malloc ((siz + 1) * sizeof(char));
	if (!s)
		return (NULL);
	if (nb < 0)
	{
		nb *= -1;
		s[0] = '-';
		ft_fill(&(s[1]), nb, siz - 1);
	}
	else
		ft_fill(s, nb, siz);
	s[siz] = '\0';
	return (s);
}
