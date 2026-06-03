/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_last_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:50:20 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/13 15:27:14 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	end_with_pipe(char *input)
{
	int		i;

	i = ft_strlen(input) - 1;
	while (i >= 0 && input[i] && input[i] == ' ')
		i--;
	if (i >= 0 && input[i] == '|')
		return (1);
	return (0);
}

void	handle_end_with_pipe(char **input, t_data *data)
{
	if (!input || !(*input) || !**input || g_sig_status == 4)
		return ;
	if (end_with_pipe(*input))
	{
		here_doc_word('\n', data);
		handle_end_with_pipe(input, data);
	}
}
