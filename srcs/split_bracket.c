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

t_token	*create_clean_list(t_token **tokens)
{
	t_token	*clean_list;
	t_token	*sub_list;
	t_token	*node;
	int		brack;

	clean_list = NULL;
	sub_list = NULL;
	brack = 0;
	while (*tokens)
	{
		if ((*tokens)->type != TOKEN_O_BRACK
			&& (*tokens)->type != TOKEN_C_BRACK)
		{
			node = lexer((*tokens)->value);
			ft_tokadd_back(&clean_list, node);
			*tokens = (*tokens)->next;
		}
		else if ((*tokens)->type == TOKEN_O_BRACK)
		{
			brack++;
			*tokens = (*tokens)->next;
			sub_list = create_clean_list(tokens);
			add_sublist(&clean_list, sub_list);
		}
		else
		{
			brack--;
			*tokens = (*tokens)->next;
			if (brack == 0)
				continue ;
			return (clean_list);
		}
	}
	return (clean_list);
}

int	check_brackets(t_token	*tokens)
{
	int	o_brack;
	int	c_brack;
	int	last_brack;

	o_brack = 0;
	c_brack = 0;
	last_brack = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_O_BRACK)
			o_brack++;
		if (tokens->type == TOKEN_C_BRACK)
			c_brack++;
		if (c_brack > o_brack)
		{
			ft_putstr_fd("minishell: ", 2), ft_putendl_fd(
				"syntax error near unexpected token `)'", 2);
			return (0);
		}
		last_brack = tokens->type;
		tokens = tokens->next;
	}
	if (o_brack == c_brack)
		return (1);
	if (last_brack == TOKEN_C_BRACK)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putendl_fd("syntax error near unexpected token `('", 2);
	}
	// else
	// 	//heredoc
	return (0);
}

t_token	*split_bracket(t_token **tokens)
{
	t_token	*clean_list;

	if (!check_brackets(*tokens))
		return (NULL);
	clean_list = create_clean_list(tokens);
	return (clean_list);
}
