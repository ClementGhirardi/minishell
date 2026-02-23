/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:22:29 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/17 22:19:40 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_isin(char c, char *s)
{
	while (*s)
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

size_t	ft_flaglen(const char *restrict format)
{
	size_t	len;

	len = 0;
	while (!ft_isin(format[len], "cspdiuxX%%"))
		len++;
	return (len);
}

int	ft_write_bspaces(long int bspaces)
{
	long int	i;

	i = 0;
	while (i < bspaces)
	{
		if (write(1, " ", 1) == -1)
			return (-1);
		i++;
	}
	return (i);
}

int	ft_write_hex(unsigned int nb)
{
	char	*hexbase;

	hexbase = "0123456789abcdef";
	if (nb > 15)
		ft_write_hex(nb / 16);
	if (write(1, &hexbase[nb % 16], 1) == -1)
		return (-1);
	return (1);
}

int	ft_write_dec(long int nb)
{
	char	*decbase;

	decbase = "0123456789";
	if (nb > 9)
		ft_write_dec(nb / 10);
	if (write(1, &decbase[nb % 10], 1) == -1)
		return (-1);
	return (1);
}
