/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:59:49 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 17:18:40 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_special_character(char c)
{
	return (c == ' '
		|| c == '|'
		|| c == '&'
		|| c == '<'
		|| c == '>'
		|| c == '('
		|| c == ')');
}

static void	error_no_space_bracket(char *input, int *status)
{
	char	*msg;
	int		i;
	int		start;

	i = 0;
	if (!input)
		return ;
	start = ++i;
	while (input[i] && !is_special_character(input[i]))
		i++;
	if (start == i)
		i++;
	msg = ft_substr(input, start, i - start);
	if (!msg)
		return ;
	syntax_error(msg, status);
	return ;
}

int	no_space_bracket(char *input, int *status)
{
	int	i;

	i = 0;
	if (!input)
		return (0);
	while (input[i])
	{
		if (input[i] == '(')
		{
			if (i > 0 && !is_special_character(input[i - 1]))
				return (error_no_space_bracket(&input[i], status), 0);
		}
		else if (input[i] == ')')
		{
			if (input[i + 1] && !is_special_character(input[i + 1]))
				return (error_no_space_bracket(&input[i], status), 0);
		}
		i++;
	}
	return (1);
}
