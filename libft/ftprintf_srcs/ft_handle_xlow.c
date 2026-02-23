/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_xlow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 10:10:18 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 17:59:38 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_write_sr(t_flag *flags, long int arg)
{
	int	i;

	i = ft_hexnbrlen(flags, arg);
	if (flags->htag == 1)
	{
		if (write(1, "0x", 2) == -1)
			return (-1);
	}
	while (i < flags->len)
	{
		if (write(1, "0", 1) == -1)
			return (-1);
		i++;
	}
	if (arg != 0 || flags->len != 0)
	{
		if (ft_write_hex(arg) == -1)
			return (-1);
	}
	if (ft_write_bspaces(flags->bspaces) == -1)
		return (-1);
	return (1);
}

static int	ft_write_snr(t_flag *flags, int arg)
{
	int	i;

	if (ft_write_bspaces(flags->bspaces) == -1)
		return (-1);
	i = ft_hexnbrlen(flags, arg);
	if (flags->htag == 1)
	{
		if (write(1, "0x", 2) == -1)
			return (-1);
	}
	while (i < flags->len)
	{
		if (write(1, "0", 1) == -1)
			return (-1);
		i++;
	}
	if (arg != 0 || flags->len != 0)
	{
		if (ft_write_hex(arg) == -1)
			return (-1);
	}
	return (1);
}

static void	ft_check(t_flag *flags, unsigned int arg)
{
	if (arg == 0)
	{
		if (flags->htag == 1 && flags->len == 2)
			flags->len = -1;
		if (flags->htag == 1 && flags->len != -1)
			flags->len -= 2;
		flags->htag = 0;
	}
	if (flags->len == -1 || (flags->len == 2 && flags->htag == 1))
	{
		if (ft_hexcondition(flags, arg))
			flags->len = ft_hexnbrlen(flags, arg);
		if (flags->zp && flags->bspaces > flags->len)
			flags->len = flags->bspaces;
	}
	else
	{
		if (ft_hexcondition(flags, arg))
			flags->len = ft_hexnbrlen(flags, arg);
	}
}

static int	ft_write_xlow(t_flag *flags, unsigned int arg)
{
	long int	count;

	ft_check(flags, arg);
	flags->bspaces -= flags->len;
	if (flags->r)
	{
		if (ft_write_sr(flags, arg) == -1)
			return (-1);
	}
	else
	{
		if (ft_write_snr(flags, arg) == -1)
			return (-1);
	}
	if (flags->bspaces + flags->len < flags->len)
		count = flags->len;
	else
		count = flags->bspaces + flags->len;
	return (count);
}

int	ft_handle_xlow(const char *format, va_list args)
{
	t_flag		flags;
	size_t		i;

	if (ft_initialize(&flags, &i) == -1)
		return (-1);
	while (ft_isin(format[i], "-.#0123456789*"))
	{
		if (format[i] == '-')
			ft_hminus(&flags);
		if (format[i] == '0')
			ft_hzero(&flags);
		if (format[i] == '#')
			ft_hhtag(&flags);
		if (format[i] == '*')
			ft_hstar(args, &flags);
		if (ft_isin(format[i], "123456789"))
			ft_hnum(format, &flags, &i);
		if (format[i] == '.')
			ft_hpoint(format, args, &flags, &i);
		i++;
	}
	return (ft_write_xlow(&flags, va_arg(args, unsigned int)));
}
