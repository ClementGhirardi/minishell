/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_end_with_pipe.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:50:20 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 15:47:25 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	before_pipe(char *input)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		j = 0;
		if (input[i] == '|')
		{
			j = i;
			if (i)
			{
				while (--i && input[i] == ' ')
					;
				if (!i || ft_is_in(input[i], "|<>"))
					return (1);
			}
			i = j;
		}
		i++;
	}
	return (0);
}

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
			else if (consecutive == 1)
				return (1);
		}
		else if (input[i] != ' ')
			consecutive = 0;
		i++;
	}
	return (0);
}

static int	only_one_pipe(char *input)
{
	int	pipe;
	int	c;
	int	i;

	pipe = 0;
	c = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			pipe++;
		else if (input[i] != ' ')
			c = 1;
		i++;
	}
	if (pipe == 1 && c == 0)
		return (1);
	return (0);
}

static int	end_with_pipe(char *input)
{
	int		i;

	i = ft_strlen(input) - 1;
	while (i >= 0 && input[i] && input[i] == ' ')
		i--;
	if (i >= 0 && input[i] == '|')
		return (1);
	return (0);
}

void	handle_end_with_pipe(char **input, t_data *data)
{
	if (!input || !(*input) || g_sig_status == 4)
		return ;
	if (consecutive_pipe(*input) || only_one_pipe(*input)
		|| before_pipe(*input))
		return ;
	if (end_with_pipe(*input))
	{
		here_doc_word('\n', data);
		handle_end_with_pipe(input, data);
	}
}
