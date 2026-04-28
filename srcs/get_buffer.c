/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_buffer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:12:40 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/28 12:12:59 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_buffer(char **buffer, int *nb_line, int status, char **env)
{
	write(1, "> ", 2);
	*buffer = get_next_line(STDIN_FILENO);
	if (!(*buffer))
		return (ft_printf("\nerror: %d\n", *nb_line), 0);
	*buffer = expand_string(*buffer, status, env);
	(*nb_line)++;
	return (1);
}
