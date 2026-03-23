#include "../includes/minishell.h"

// t_token	*duplist(t_token *tokens_bracket)
// {
// 	t_token	*node;

// 	node = malloc(sizeof(t_token));
// 	if (!node)
// 		return (NULL);
// 	node->type = tokens_bracket->type;
// 	node->value = ft_strdup(tokens_bracket->value);
// 	node->next = NULL;
// 	return (node);
// }

// t_token	*fill_clean_list(t_token **clean_list, t_token *tokens_bracket)
// {
// 	t_token	*node;
// 	int		i;

// 	i = 0;
// 	while (tokens_bracket->next)
// 	{
// 		node = duplist(tokens_bracket);
// 		lstadd_back(clean_list, node);
// 		tokens_bracket = tokens_bracket->next;
// 	}
// 	return (*clean_list);
// }

// t_token	*check_brackets_validity(t_token **tokens_bracket)
// {
// 	t_token	*clean_list;
// 	t_token	*tmp;
// 	int		o_brack;
// 	int		c_brack;

// 	tmp = *tokens_bracket;
// 	o_brack = 1;
// 	c_brack = 0;
// 	while (tmp->next)
// 	{
// 		if (tmp->type == TOKEN_O_BRACK)
// 			o_brack++;
// 		if (tmp->type == TOKEN_C_BRACK)
// 			c_brack++;
// 		tmp = tmp->next;
// 	}
// 	if (o_brack != c_brack)
// 		return (NULL);
// 	return (fill_clean_list(&clean_list, tokens_bracket));
// }

/* Pas termine */
t_token	**one_pair_of_brackets(t_token **tokens)
{
	t_token	*clean_list;
	t_token	*tmp;
	int		o_brack;
	int		c_brack;

	o_brack = 1;
	c_brack = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_O_BRACK)
		{
			tmp = tmp->next;
			clean_list = check_brackets_validity(&tmp);
			tmp->brackets = clean_list;
			while (o_brack != c_brack)
			{
				if (tmp->type == TOKEN_O_BRACK)
					o_brack++;
				if (tmp->type == TOKEN_C_BRACK)
					c_brack++;
				tmp = tmp->next;
			}
		}
		else
			tmp = tmp->next;
	}
	return (tokens);
}

void	add_sublist(t_token **clean_list, t_token *sub_list)
{
	t_token	*tmp;

	tmp = (*clean_list);
	while (tmp->next)
		tmp = tmp->next;
	tmp->brackets = sub_list;
}

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

/* split_bracket avant lexer, t_token *create_list(char *input) ? */
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
			lstadd_back(&clean_list, node);
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

// t_token	*split_bracket(t_token **tokens)
// {
// 	t_token	*tmp;

// 	tmp = *tokens;
// 	while (tmp)
// 	{
// 		if (tmp->type == TOKEN_O_BRACK)
// 			break ;
// 		tmp = tmp->next;
// 	}
// 	if (tmp->type != TOKEN_O_BRACK)
// 		return (tokens);
// 	tmp = tmp->next;
// 	return (create_clean_list(&tmp));
// }
