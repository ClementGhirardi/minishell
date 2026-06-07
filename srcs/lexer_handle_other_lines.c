/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_other_lines.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:33:21 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/04 18:26:26 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	create_other_lines(char *input, t_token **tokens, int *i, int start)
{
	create_word(input, tokens, *i, start);
	start = *i + 1;
	*i = ft_strlen(input);
	create_word(input, tokens, *i, start);
}

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
