/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 14:03:02 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 15:23:24 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_nbrlen(t_flag *flags, int nb)
{
	long int	nbr;
	int			len;

	nbr = (long int) nb;
	len = 1;
	if (nbr < 0)
	{
		len++;
		nbr *= -1;
	}
	if ((len == 1) && (flags->blsign == 1 || flags->sign == 1))
		len++;
	while (nbr > 9)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

int	ft_write_sign(t_flag *flags, long int *arg)
{
	if (*arg < 0)
	{
		*arg *= -1;
		if (write(1, "-", 1) == -1)
			return (-1);
	}
	else if (flags->sign)
	{
		if (write(1, "+", 1) == -1)
			return (-1);
	}
	else if (flags->blsign)
	{
		if (write(1, " ", 1) == -1)
			return (-1);
	}
	return (1);
}

int	ft_unbrlen(unsigned int nb)
{
	int			len;

	len = 1;
	while (nb > 9)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

int	ft_write_udec(unsigned int nb)
{
	char	*decbase;

	decbase = "0123456789";
	if (nb > 9)
		ft_write_dec(nb / 10);
	if (write(1, &decbase[nb % 10], 1) == -1)
		return (-1);
	return (1);
}

int	ft_hexnbrlen(t_flag *flags, unsigned int nb)
{
	int			len;

	len = 1;
	if (flags->htag)
		len += 2;
	while (nb > 15)
	{
		nb /= 16;
		len++;
	}
	return (len);
}
