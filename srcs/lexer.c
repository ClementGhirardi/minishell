/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:53:34 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 18:07:22 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token	*fill_tokens(char **input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == ' ' || (*input)[i] == '\t')
			i++;
		else if ((*input)[i] == '&')
			handle_and(*input, &tokens, &i);
		else if ((*input)[i] == '|')
			handle_pipe(*input, &tokens, &i);
		else if ((*input)[i] == '>')
			handle_redir(*input, &tokens, &i, 1);
		else if ((*input)[i] == '<')
			handle_redir(*input, &tokens, &i, 0);
		else if ((*input)[i] == '(' || (*input)[i] == ')')
			handle_bracket(*input, &tokens, &i);
		else
			handle_word(*input, &tokens, &i);
	}
	return (tokens);
}

t_token	*lexer(char **input, t_data *data)
{
	t_token	*tokens;

	data->end_with_pipe = end_with_pipe(*input);
	if (!input || !*input || !**input)
		return (NULL);
	if (handle_quotes(input) || !no_space_bracket(*input, data->status))
		return (NULL);
	tokens = fill_tokens(input);
	if (!syntax_analyzer(tokens))
		return (free_token(tokens), NULL);
	lexer_handle_other_lines(tokens, data);
	return (data->tokens = tokens, tokens);
}
