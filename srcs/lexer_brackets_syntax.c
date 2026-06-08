/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_brackets_syntax.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 11:34:47 by adbarth           #+#    #+#             */
/*   Updated: 2026/06/08 11:10:52 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*syntax_error(char *c)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(c, 2);
	ft_putendl_fd("'", 2);
	return (NULL);
}

int	syntax_after_obracket(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type != TOKEN_WORD
		&& tokens->type != TOKEN_O_BRACK
		&& tokens->type != TOKEN_C_BRACK)
		return (0);
	return (1);
}

t_token	*check_brackets(t_token	*tokens)
{
	t_token	*tmp;
	int		o_brack;
	int		c_brack;

	o_brack = 0;
	c_brack = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_O_BRACK)
			o_brack++;
		if (tmp->type == TOKEN_C_BRACK)
			c_brack++;
		if (c_brack > o_brack)
			return (syntax_error(")"));
		tmp = tmp->next;
	}
	return (tokens);
}
