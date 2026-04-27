/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_last_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:50:20 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/27 16:50:34 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_last_pipe(char **input)
{
	int	i;

	if (!(*input))
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
