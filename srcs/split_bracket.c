#include "../includes/minishell.h"

t_token	*duplist(t_token *tokens_bracket)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->type = tokens_bracket->type;
	node->value = ft_strdup(tokens_bracket->value);
	node->next = NULL;
	return (node);
}

t_token	*fill_clean_list(t_token **clean_list, t_token *tokens_bracket)
{
	t_token	*node;
	int		i;

	i = 0;
	while (tokens_bracket->next)
	{
		node = duplist(tokens_bracket);
		lstadd_back(clean_list, node);
		tokens_bracket = tokens_bracket->next;
	}
	return (*clean_list);
}

t_token	*create_clean_list(t_token **tokens_bracket)
{
	t_token	*clean_list;
	t_token	*tmp;
	int		o_brack;
	int		c_brack;

	tmp = *tokens_bracket;
	o_brack = 0;
	c_brack = 0;
	while (tmp->next)
	{
		if (tmp->type == TOKEN_O_BRACK)
			o_brack++;
		if (tmp->type == TOKEN_C_BRACK)
			c_brack++;
		tmp = tmp->next;
	}
	if (o_brack != c_brack)
		return (NULL);
	return (fill_clean_list(&clean_list, tokens_bracket));
}

t_token	*split_bracket(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_O_BRACK)
			break ;
		tmp = tmp->next;
	}
	if (tmp->type != TOKEN_O_BRACK)
		return (tokens);
	tmp = tmp->next;
	return (create_clean_list(&tmp));
}
