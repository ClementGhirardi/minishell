/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_last_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:50:20 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/28 13:29:20 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	consecutive_pipe(char *input)
{
	int	consecutive;
	int	i;

	consecutive = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (consecutive == 0)
				consecutive = 1;
			if (consecutive == 1)
				return (1);
		}
		else if (input[i] != ' ')
			consecutive = 0;
		i++;
	}
	return (0);
}

void	handle_last_pipe(char **input)
{
	int	i;

	if (!(*input))
		return ;
	if (consecutive_pipe(*input))
		return ;
	i = ft_strlen(*input) - 1;
	while ((*input)[i] && (*input)[i] == ' ')
		i--;
	if (i >= 0 && (*input)[i] == '|')
		*input = ft_strjoin_and_free(*input,
				ft_strjoin_and_free(ft_strdup(" "), here_doc_word('\n')));
	else
		return ;
	lexer(input);
}
