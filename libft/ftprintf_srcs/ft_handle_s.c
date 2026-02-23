/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_s.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 10:10:34 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 15:32:14 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_write_sr(t_flag *flags, char *arg)
{
	if (!arg && flags->len == 6)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
	}
	else
	{
		if (write(1, arg, flags->len) == -1)
			return (-1);
	}
	if (ft_write_bspaces(flags->bspaces) == -1)
		return (-1);
	return (1);
}

static int	ft_write_snr(t_flag *flags, char *arg)
{
	if (ft_write_bspaces(flags->bspaces) == -1)
		return (-1);
	if (!arg && flags->len == 6)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
	}
	else
	{
		if (write(1, arg, flags->len) == -1)
			return (-1);
	}
	return (1);
}

static void	ft_check(t_flag *flags, char *arg)
{
	if (flags->blsign == 1 && *arg != '\0' && flags->len == 1)
		flags->len = -1;
	if (!arg && (flags->len == -1 || flags->len >= 6))
		flags->len = 6;
	else if (!arg && flags->len < 6)
		flags->len = 0;
	else if (flags->len == -1 || flags->len > (int) ft_strlen(arg))
		flags->len = (int) ft_strlen(arg);
	flags->bspaces -= flags->len;
}

static int	ft_write_s(t_flag *flags, char *arg)
{
	long int	count;

	ft_check(flags, arg);
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

int	ft_handle_s(const char *format, va_list args)
{
	t_flag		flags;
	size_t		i;

	flags.r = 0;
	flags.bspaces = 0;
	flags.len = -1;
	flags.zp = 0;
	flags.htag = 0;
	flags.blsign = 0;
	i = 0;
	while (ft_isin(format[i], "-. 123456789*"))
	{
		if (format[i] == '-')
			ft_hminus(&flags);
		if (format[i] == '*')
			ft_hstar(args, &flags);
		if (format[i] == ' ')
			ft_hspace(&flags);
		if (ft_isin(format[i], "123456789"))
			ft_hnum(format, &flags, &i);
		if (format[i] == '.')
			ft_hpoint(format, args, &flags, &i);
		i++;
	}
	return (ft_write_s(&flags, va_arg(args, char *)));
}
