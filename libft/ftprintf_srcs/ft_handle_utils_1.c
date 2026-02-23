/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:26:57 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 16:18:49 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_hpoint_condition(const char *format, t_flag *flags, size_t *i)
{
	size_t	j;

	if (flags->len == -1)
		flags->len = ft_atoi(&format[*i + 1]);
	else
		flags->len += ft_atoi(&format[*i + 1]);
	j = *i;
	if (ft_isin(format[*i + 1], "0123456789"))
		(*i)++;
	while (ft_isin(format[*i], "0123456789"))
		(*i)++;
	if (j != *i)
		(*i)--;
}

void	ft_hpoint(const char *format, va_list args, t_flag *flags, size_t *i)
{
	size_t	arg;

	if (format[*i + 1] == '*')
	{
		arg = va_arg(args, int);
		if (flags->len == -1)
			flags->len = arg + (arg == 0);
		else
			flags->len += arg + (arg == 0);
	}
	else
		ft_hpoint_condition(format, flags, i);
	if (flags->zp == 1)
		flags->zp = 0;
}

void	ft_hspace(t_flag *flags)
{
	flags->blsign = 1;
	flags->len = 1;
}

void	ft_hplus(t_flag *flags)
{
	flags->sign = 1;
	flags->len = 1;
}

void	ft_hhtag(t_flag *flags)
{
	flags->htag = 1;
	flags->len = 2;
}
