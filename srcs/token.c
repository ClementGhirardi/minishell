/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:39:28 by cghirard          #+#    #+#             */
/*   Updated: 2026/04/27 11:16:36 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(1 * sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
		return (free(token), NULL);
	token->bracket = NULL;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (!tokens)
		return ;
	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	free_token(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		if (tokens->value)
			free(tokens->value);
		if (tokens->bracket)
			free_token(tokens->bracket);
		tmp = tokens->next;
		free(tokens);
		tokens = tmp;
	}
}
