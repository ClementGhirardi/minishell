/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_bracket.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbarth <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 11:34:47 by adbarth           #+#    #+#             */
/*   Updated: 2026/04/17 11:34:51 by adbarth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_tokadd_back(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current)
	{
		if (current->next == NULL)
		{
			current->next = new;
			return ;
		}
		current = current->next;
	}
	return ;
}

void	add_sublist(t_token **clean_list, t_token *sub_list)
{
	t_token	*tmp;
	t_token	*empty;

	if (!*clean_list)
	{
		empty = new_token(TOKEN_WORD, "");
		ft_tokadd_back(clean_list, empty);
	}
	tmp = *clean_list;
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->bracket = sub_list;
}

void	handle_non_brackets(t_token **tokens, t_token **clean_list)
{
	t_token	*node;

	if ((*tokens)->type != TOKEN_O_BRACK
		&& (*tokens)->type != TOKEN_C_BRACK)
	{
		node = new_token((*tokens)->type, (*tokens)->value);
		ft_tokadd_back(clean_list, node);
		*tokens = (*tokens)->next;
	}
}

t_token	*create_clean_list(t_token **tokens)
{
	t_token	*clean_list;
	t_token	*sub_list;

	clean_list = NULL;
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_O_BRACK)
		{
			*tokens = (*tokens)->next;
			sub_list = create_clean_list(tokens);
			add_sublist(&clean_list, sub_list);
		}
		else if ((*tokens)->type == TOKEN_C_BRACK)
		{
			*tokens = (*tokens)->next;
			return (clean_list);
		}
		else
			handle_non_brackets(tokens, &clean_list);
	}
	return (clean_list);
}

t_token	*split_bracket(t_token **tokens)
{
	t_token	*clean_list;
	t_token	*tmp;

	if (!tokens || !*tokens)
		return (NULL);
	tmp = *tokens;
	clean_list = create_clean_list(tokens);
	free_token(tmp);
	return (clean_list);
}
