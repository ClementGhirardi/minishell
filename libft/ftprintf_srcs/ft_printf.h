/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 11:58:06 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/24 14:31:49 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "../libft.h"
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_flag
{
	int			r;
	int			zp;
	long int	bspaces;
	long int	len;
	int			htag;
	int			blsign;
	int			sign;
}	t_flag;

size_t	ft_isin(char c, char *s);
size_t	ft_flaglen(const char *format);
int		ft_write_bspaces(long int bspaces);
int		ft_write_hex(unsigned int nb);
int		ft_write_dec(long int nb);
int		ft_nbrlen(t_flag *flags, int nb);
int		ft_write_sign(t_flag *flags, long int *arg);
int		ft_unbrlen(unsigned int nb);
int		ft_write_udec(unsigned int nb);
int		ft_hexnbrlen(t_flag *flags, unsigned int nb);
int		ft_initialize(t_flag *flags, size_t *i);
int		ft_write_hexup(unsigned int nb);
int		ft_write_addr(unsigned long nb);
int		ft_hexcondition(t_flag *flags, unsigned int arg);
int		ft_deccondition(t_flag *flags, long int arg);
void	ft_hminus(t_flag *flags);
void	ft_hstar(va_list args, t_flag *flags);
void	ft_hnum(const char *format, t_flag *flags, size_t *i);
void	ft_hpoint(const char *format, va_list args, t_flag *flags, size_t *i);
void	ft_hzero(t_flag *flags);
void	ft_hspace(t_flag *flags);
void	ft_hplus(t_flag *flags);
void	ft_hhtag(t_flag *flags);
int		ft_handle_c(const char *format, va_list args);
int		ft_handle_s(const char *format, va_list args);
int		ft_handle_p(const char *format, va_list args);
int		ft_handle_d(const char *format, va_list args);
int		ft_handle_i(const char *format, va_list args);
int		ft_handle_u(const char *format, va_list args);
int		ft_handle_xlow(const char *format, va_list args);
int		ft_handle_xup(const char *format, va_list args);
int		ft_handle_perc(const char *format, va_list args);

#endif