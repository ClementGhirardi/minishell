/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_utils_0.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:09:15 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 15:29:22 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_hminus(t_flag *flags)
{
	flags->r = 1;
}

void	ft_hstar(va_list args, t_flag *flags)
{
	size_t	arg;

	arg = va_arg(args, int);
	flags->bspaces = arg + (arg == 0);
}

void	ft_hnum(const char *format, t_flag *flags, size_t *i)
{
	size_t	j;

	j = *i;
	flags->bspaces = ft_atoi(&format[*i]);
	while (ft_isin(format[*i], "0123456789"))
		(*i)++;
	if (j != *i)
		(*i)--;
}

void	ft_hzero(t_flag *flags)
{
	flags->zp = 1;
}
