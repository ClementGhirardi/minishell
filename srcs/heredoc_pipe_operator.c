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

char	*here_doc_pipe_op(char *input_beginning, int *status, char **env)
{
	char	**history;
	char	*to_add;
	char	*input;

	history = ft_calloc(2, sizeof(char *) * 3);
	if (!history)
		return (NULL);
	history[0] = ft_strdup(input_beginning);
	free(input_beginning);
	history[2] = NULL;
	input = readline("> ");
	if (!input)
		error_heredocword(0, *status, env);
	while (input && isempty(input))
	{
		free(input);
		input = readline("> ");
		if (!input)
			error_heredocword(0, *status, env);
	}
	history[1] = ft_strjoin_and_free(ft_strdup(" "), ft_strdup(input));
	to_add = ft_strjoin_and_free(history[0], history[1]);
	add_history(to_add);
	return (free(history), free(to_add), input);
}
