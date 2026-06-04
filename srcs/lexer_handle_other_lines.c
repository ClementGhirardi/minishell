/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_other_lines.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:33:21 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 15:16:29 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lexer_handle_other_lines(t_token *tokens, t_data *data)
{
	t_token	*current;
	t_token	*previous;

	if (!tokens)
		return ;
	data->other_lines = NULL;
	previous = tokens;
	current = tokens->next;
	while (current)
	{
		if (current->type == TOKEN_WORD && !current->next)
		{
			if (ft_is_in('\n', current->value))
			{
				data->other_lines = ft_strdup(current->value);
				free_token(current);
				previous->next = NULL;
				return ;
			}
		}
		previous = current;
		current = current->next;
	}
}
