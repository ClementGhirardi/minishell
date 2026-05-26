/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_last_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:50:20 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/13 15:27:14 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	consecutive_pipe(char *input)
// {
// 	int	consecutive;
// 	int	i;

// 	consecutive = 0;
// 	i = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '|')
// 		{
// 			if (consecutive == 0)
// 				consecutive = 1;
// 			else if (consecutive == 1)
// 				return (1);
// 		}
// 		else if (input[i] != ' ')
// 			consecutive = 0;
// 		i++;
// 	}
// 	return (0);
// }

// static int	only_one_pipe(char *input)
// {
// 	int	pipe;
// 	int	c;
// 	int	i;

// 	pipe = 0;
// 	c = 0;
// 	i = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '|')
// 			pipe++;
// 		else if (input[i] != ' ')
// 			c = 1;
// 		i++;
// 	}
// 	if (pipe == 1 && c == 0)
// 		return (1);
// 	return (0);
// }

// void	handle_last_pipe(char **input, int *status, char **env)
// {
// 	int		i;

// 	if (!input || !(*input) || g_sig_status == 4)
// 		return ;
// 	if (consecutive_pipe(*input) || only_one_pipe(*input))
// 		return ;
// 	i = ft_strlen(*input) - 1;
// 	while (i >= 0 && (*input)[i] && (*input)[i] == ' ')
// 		i--;
// 	if (i >= 0 && (*input)[i] == '|')
// 	{
// 		here_doc_word(input, '\n', status, env);
// 		handle_last_pipe(input, status, env);
// 	}
// 	if (g_sig_status == 4)
// 		add_history(*input);
// }


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

t_token	*last_pipe(char *input, t_token *tokens, int *status, char **env)
{
	t_token	*tmp;
	t_token	*end_token;
	char	*heredoc_output;

	if (!tokens)
		return (NULL);
	tmp = tokens;
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->type == TOKEN_PIPE)
	{
		heredoc_output = ft_strjoin_and_free(ft_strdup(" "),
				here_doc_word(&input, '\n', status, env));
		end_token = lexer(&heredoc_output, status, env);
		free(heredoc_output);
		return (end_token);
	}
	return (NULL);
}
