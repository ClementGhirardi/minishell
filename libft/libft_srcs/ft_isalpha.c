/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:48:01 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/12 14:42:07 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isupper(int c)
{
	if (('A' <= c) && (c <= 'Z'))
		return (256);
	return (0);
}

static int	ft_islower(int c)
{
	if (('a' <= c) && (c <= 'z'))
		return (512);
	return (0);
}

int	ft_isalpha(int c)
{
	if (ft_isupper(c) || ft_islower(c))
		return (1024);
	return (0);
}
