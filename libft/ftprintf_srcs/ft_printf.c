/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 11:56:13 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/18 13:29:48 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_handle(const char *format, va_list args)
{
	size_t	i;

	i = 0;
	while (ft_isin(format[i], "-0.# +123456789*"))
		i++;
	if (format[i] == 'c')
		return (ft_handle_c(format, args));
	else if (format[i] == 's')
		return (ft_handle_s(format, args));
	else if (format[i] == 'p')
		return (ft_handle_p(format, args));
	else if (format[i] == 'd')
		return (ft_handle_d(format, args));
	else if (format[i] == 'i')
		return (ft_handle_i(format, args));
	else if (format[i] == 'u')
		return (ft_handle_u(format, args));
	else if (format[i] == 'x')
		return (ft_handle_xlow(format, args));
	else if (format[i] == 'X')
		return (ft_handle_xup(format, args));
	else if (format[i] == '%')
		return (ft_handle_perc(format, args));
	return (0);
}

static void	ft_init(size_t *count, size_t *i)
{
	*count = 0;
	*i = 0;
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	size_t	count;
	size_t	i;
	int		verif;

	va_start(args, format);
	ft_init(&count, &i);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			verif = ft_handle(&format[i], args);
			if (verif == -1)
				return (-1);
			count += verif - 1;
			i += ft_flaglen(&format[i]);
		}
		else
			write(1, &format[i], 1);
		count++;
		i++;
	}
	va_end(args);
	return (count);
}
