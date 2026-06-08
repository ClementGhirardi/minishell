/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strslen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 15:58:57 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 15:59:08 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strslen(char **strs)
{
	int	len;

	len = 0;
	if (!strs)
		return (len);
	while (strs[len])
		len++;
	return (len);
}
