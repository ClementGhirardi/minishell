#include "../includes/minishell.h"

int	is_valid_variable(char *arg, char **env)
{
	char	*tmp;
	char	*arg_copy;

	arg_copy = ft_strdup(arg);
	if (!arg)
		return (0);
	tmp = expand_string(arg_copy, env);
	if (!tmp)
		return (0);
	free(tmp);
	return (1);
}

char	**alloc_array(char **array, char **env)
{
	char	**new;
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (array[i])
	{
		if (is_valid_variable(array[i], env))
			count++;
		i++;
	}
	new = malloc(sizeof(char *) * (count + 1));
	if (!new)
		return (NULL);
	return (new);
}

char	**filter_and_dup_array(char **array, char **env)
{
	char	**new;
	int		i;
	int		j;

	i = -1;
	j = -1;
	new = alloc_array(array, env);
	if (!new)
		return (NULL);
	while (array[++i])
	{
		if (!array[i])
			return (NULL);
		if (is_valid_variable(array[i], env))
		{
			new[++j] = expand_string(array[i], env);
			if (!new[j])
			{
				free_array(new);
				return (NULL);
			}
		}
	}
	new[++j] = NULL;
	return (new);
}
