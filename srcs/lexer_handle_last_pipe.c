/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_last_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:50:20 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 13:50:30 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	handle_last_pipe(char **input, t_data *data)
{
	if (!input || !(*input) || g_sig_status == 4)
		return ;
	if (consecutive_pipe(*input) || only_one_pipe(*input))
		return ;
	if (end_with_pipe(*input))
	{
		here_doc_word('\n', data);
		handle_last_pipe(input, data);
	}
}






// void	ft_tokadd_back(t_token **lst, t_token *new)
// {
// 	t_token	*current;

// 	if (!lst || !new)
// 		return ;
// 	if (!(*lst))
// 	{
// 		*lst = new;
// 		return ;
// 	}
// 	current = *lst;
// 	while (current)
// 	{
// 		if (current->next == NULL)
// 		{
// 			current->next = new;
// 			return ;
// 		}
// 		current = current->next;
// 	}
// 	return ;
// }

// // t_token	*last_pipe(char *input, t_token *tokens, int *status, char **env)
// t_token	*last_pipe(t_token *tokens, t_data *data)
// {
// 	t_token	*tmp;
// 	t_token	*end_token;
// 	char	*heredoc_output;

// 	if (!tokens || !data)
// 		return (NULL);
// 	tmp = tokens;
// 	while (tmp->next)
// 		tmp = tmp->next;
// 	if (tmp->type == TOKEN_PIPE)
// 	{
// 		// heredoc_output = ft_strjoin_and_free(ft_strdup(" "),
// 		// 		here_doc_word('\n', data));
// 		heredoc_output = here_doc_word('\n', data);
// 		end_token = lexer(data, &heredoc_output);
// 		free(heredoc_output);
// 		return (end_token);
// 	}
// 	return (NULL);
// }
