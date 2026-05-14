#include "../includes/minishell.h"

int	syntax_after_cbracket(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (tokens->type != TOKEN_WORD
		&& tokens->type != TOKEN_O_BRACK
		&& tokens->type != TOKEN_C_BRACK)
		return (0);
	return (1);
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

int	syntax_after_redir(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_REDIR_IN
		|| tokens->type == TOKEN_REDIR_OUT
		|| tokens->type == TOKEN_HEREDOC
		|| tokens->type == TOKEN_APPEND)
		return (0);
	return (1);
}

int	syntax_after_token(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (tokens->type == TOKEN_AND
		|| tokens->type == TOKEN_OR
		|| tokens->type == TOKEN_PIPE)
		return (0);
	return (1);
}

int	dispatch(t_token *tokens)
{
	if (tokens->type != TOKEN_WORD
		&& tokens->type != TOKEN_O_BRACK
		&& tokens->type != TOKEN_C_BRACK
		&& !syntax_after_token(tokens->next))
		return (0);
	if ((tokens->type == TOKEN_REDIR_IN
			|| tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_HEREDOC
			|| tokens->type == TOKEN_APPEND)
		&& !syntax_after_redir(tokens->next))
		return (0);
	if (tokens->type == TOKEN_O_BRACK)
	{
		if (tokens->next->type == TOKEN_C_BRACK)
			return (0);
		return (syntax_after_obracket(tokens->next));
	}
	if (tokens->type == TOKEN_C_BRACK)
		return ((tokens->next && tokens->next->type != TOKEN_O_BRACK)
			|| !tokens->next);
	return (1);
}

static t_token	*check_brackets(t_token	*tokens, int *status)
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
			return (syntax_error(")", status));
		tmp = tmp->next;
	}
	if (o_brack == c_brack)
		return (tokens);
	else
		return (syntax_error("(", status));
}

int	syntax_first_token(t_token *tokens, int *status)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_AND
		|| tokens->type == TOKEN_OR
		|| tokens->type == TOKEN_PIPE
		|| tokens->type == TOKEN_C_BRACK)
	{
		syntax_error(tokens->value, status);
		return (0);
	}
	return (1);
}

int	syntax_analyzer(t_token *tokens, int *status)
{
	if (!syntax_first_token(tokens, status) || !check_brackets(tokens, status))
		return (0);
	while (tokens)
	{
		if (!dispatch(tokens))
		{
			if (tokens->next)
				syntax_error(tokens->next->value, status);
			else
				syntax_error("newline", status);
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}
