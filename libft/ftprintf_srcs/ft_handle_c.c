/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_c.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 10:10:12 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 00:32:26 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_write_c(t_flag *flags, int arg)
{
	long int	count;

	flags->bspaces -= flags->len;
	if (flags->r)
	{
		if (write(1, &arg, 1) == -1)
			return (-1);
		if (ft_write_bspaces(flags->bspaces) == -1)
			return (-1);
	}
	else
	{
		if (ft_write_bspaces(flags->bspaces) == -1)
			return (-1);
		if (write(1, &arg, 1) == -1)
			return (-1);
	}
	count = flags->bspaces + 1;
	return (count);
}

int	ft_handle_c(const char *format, va_list args)
{
	t_flag	flags;
	size_t	i;

	flags.r = 0;
	flags.bspaces = 1;
	flags.len = 1;
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
	return (ft_write_c(&flags, va_arg(args, int)));
}
