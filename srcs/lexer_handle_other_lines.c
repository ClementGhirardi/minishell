/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_other_lines.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:33:21 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 18:03:23 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	create_other_lines(char *input, t_token **tokens, int *i, int start)
{
	if (*i != start)
		create_word(input, tokens, *i, start);
	start = *i + 1;
	*i = ft_strlen(input);
	create_word(input, tokens, *i, start);
}

static void	add_newline(t_data *data, t_token **current)
{
	if ((*current)->value[ft_strlen((*current)->value) - 1] != '\n')
	{
		(*current)->value = ft_strjoin_and_free(
				(*current)->value, ft_strdup("\n"));
		*data->input = ft_strjoin_and_free(*data->input, ft_strdup("\n"));
	}
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
				add_newline(data, &current);
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
