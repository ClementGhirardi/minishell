#include "../includes/minishell.h"

int	isempty(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

char	*here_doc_pipe_op(char **env)
{
	char	*input;

	input = readline("> ");
	if (!input)
		error_heredocword(0, env);
	while (input && isempty(input))
	{
		free(input);
		input = readline("> ");
		if (!input)
			error_heredocword(0, env);
	}
	return (input);
}