/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 10:41:53 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/12 14:48:12 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isin(char c, char *nptr)
{
	int	i;

	i = 0;
	while (nptr[i] != '\0')
	{
		if (nptr[i] == c)
			return (1);
		i ++;
	}
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	nb;
	int	sign;

	i = 0;
	nb = 0;
	sign = 1;
	while (ft_isin(nptr[i], " \t\r\n\v\f") == 1)
		i ++;
	if (ft_isin(nptr[i], "+-"))
	{
		if (nptr[i] == '-')
			sign *= -1;
		i ++;
	}
	while ('0' <= nptr[i] && nptr[i] <= '9')
	{
		nb = nb * 10 + nptr[i] - '0';
		i ++;
	}
	return (sign * nb);
}
