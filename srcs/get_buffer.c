/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_buffer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:12:40 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/29 18:51:58 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_buffer(char **buffer, int *nb_line, int status, char **env)
{
	*buffer = ft_strjoin_and_free(readline("> "), ft_strdup("\n"));
	ft_printf("end\n");
	if (!(*buffer))
		return (0);
	*buffer = expand_string(*buffer, status, env);
	(*nb_line)++;
	return (1);
}
