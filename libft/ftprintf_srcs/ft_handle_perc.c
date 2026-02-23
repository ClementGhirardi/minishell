/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_perc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 10:09:11 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 00:29:55 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_handle_perc(const char *format, va_list args)
{
	t_flag	flags;
	size_t	i;

	if (ft_initialize(&flags, &i) == -1)
		return (-1);
	while (ft_isin(format[i], "-. #0123456789*+"))
	{
		if (format[i] == '*')
			ft_hstar(args, &flags);
		i++;
	}
	if (write(1, "%%", 1) == -1)
		return (-1);
	return (1);
}
