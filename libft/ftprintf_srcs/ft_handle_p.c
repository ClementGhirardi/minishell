/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_p.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 10:10:28 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 00:32:15 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

long int	ft_addrlen(unsigned long addr)
{
	long int	len;

	len = 1;
	while (addr > 15)
	{
		len++;
		addr /= 16;
	}
	return (len);
}

static int	ft_write_pr(t_flag *flags, unsigned long arg)
{
	if (!arg)
	{
		if (write(1, "(nil)", 5) == -1)
			return (-1);
	}
	else
	{
		if (write(1, "0x", 2) == -1)
			return (-1);
		if (ft_write_addr(arg) == -1)
			return (-1);
	}
	if (ft_write_bspaces(flags->bspaces) == -1)
		return (-1);
	return (1);
}

static int	ft_write_pnr(t_flag *flags, unsigned long arg)
{
	if (ft_write_bspaces(flags->bspaces) == -1)
		return (-1);
	if (!arg)
	{
		if (write(1, "(nil)", 5) == -1)
			return (-1);
	}
	else
	{
		if (write(1, "0x", 2) == -1)
			return (-1);
		if (ft_write_addr(arg) == -1)
			return (-1);
	}
	return (1);
}

static int	ft_write_p(t_flag *flags, unsigned long arg)
{
	long int	count;

	if (!arg)
		flags->len = 5;
	else
		flags->len = ft_addrlen(arg) + 2;
	flags->bspaces -= flags->len;
	if (flags->r)
	{
		if (ft_write_pr(flags, arg) == -1)
			return (-1);
	}
	else
	{
		if (ft_write_pnr(flags, arg) == -1)
			return (-1);
	}
	if (flags->bspaces + flags->len < flags->len)
		count = flags->len;
	else
		count = flags->bspaces + flags->len;
	return (count);
}

int	ft_handle_p(const char *format, va_list args)
{
	t_flag	flags;
	size_t	i;

	flags.r = 0;
	flags.bspaces = 0;
	i = 0;
	while (ft_isin(format[i], "-.123456789*"))
	{
		if (format[i] == '-')
			ft_hminus(&flags);
		if (format[i] == '*')
			ft_hstar(args, &flags);
		if (ft_isin(format[i], "123456789"))
			ft_hnum(format, &flags, &i);
		i++;
	}
	return (ft_write_p(&flags, (unsigned long) va_arg(args, void *)));
}
