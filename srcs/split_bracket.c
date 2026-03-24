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

	tmp = (*clean_list);
	while (tmp->next)
		tmp = tmp->next;
	tmp->bracket = sub_list;
}

/* Ne gere pas encore le cas )( */
int	check_brackets_validity(t_token	*tokens)
{
	int	o_brack;
	int	c_brack;

	o_brack = 0;
	c_brack = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_O_BRACK)
			o_brack++;
		if (tokens->type == TOKEN_C_BRACK)
			c_brack++;
		tokens = tokens->next;
	}
	if (o_brack != c_brack)
		return (0);
	return (1);
}

t_token	*create_clean_list(t_token **tokens)
{
	t_token	*clean_list;
	t_token	*sub_list;
	t_token	*node;

	clean_list = NULL;
	sub_list = NULL;
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
			*tokens = (*tokens)->next;
			sub_list = create_clean_list(tokens);
			add_sublist(&clean_list, sub_list);
		}
		else
		{
			*tokens = (*tokens)->next;
			return (clean_list);
		}
	}
	return (clean_list);
}

t_token	*split_bracket(t_token *tokens)
{
	if (!check_brackets_validity(tokens))
		return (NULL);
	return (create_clean_list(&tokens));
}
