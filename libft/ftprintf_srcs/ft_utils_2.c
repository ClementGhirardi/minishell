/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:20:06 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 16:18:20 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_initialize(t_flag *flags, size_t *i)
{
	flags->r = 0;
	flags->zp = 0;
	flags->bspaces = 0;
	flags->len = -1;
	flags->blsign = 0;
	flags->sign = 0;
	flags->htag = 0;
	*i = 0;
	return (1);
}

int	ft_write_hexup(unsigned int nb)
{
	char	*hexbase;

	hexbase = "0123456789ABCDEF";
	if (nb > 15)
		ft_write_hexup(nb / 16);
	if (write(1, &hexbase[nb % 16], 1) == -1)
		return (-1);
	return (1);
}

int	ft_write_addr(unsigned long nb)
{
	char	*hexbase;

	hexbase = "0123456789abcdef";
	if (nb > 15)
		ft_write_addr(nb / 16);
	if (write(1, &hexbase[nb % 16], 1) == -1)
		return (-1);
	return (1);
}

int	ft_hexcondition(t_flag *flags, unsigned int arg)
{
	int	x;
	int	y;

	x = (flags->len == -1);
	y = (flags->len < ft_hexnbrlen(flags, arg) && arg != 0);
	return (x || y);
}

int	ft_deccondition(t_flag *flags, long int arg)
{
	int	x;
	int	y;

	x = (flags->blsign == 1 || flags->sign == 1);
	y = (flags->len >= ft_nbrlen(flags, arg));
	return (x && y);
}
