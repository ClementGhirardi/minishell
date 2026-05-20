#include "../includes/minishell.h"

static void	*ft_realloc(void *ptr, size_t size)
{
	unsigned char	*new;
	unsigned char	*p;
	size_t			i;

	if (!size)
		return (free(ptr), NULL);
	p = ptr;
	i = 0;
	new = ft_calloc(1, size);
	if (!new)
		return (NULL);
	while (p && p[i])
	{
		new[i] = p[i];
		i++;
	}
	free(ptr);
	return (new);
}

char	**remove_empty_var(char **args, int status, char **env)
{
	char	**clean;
	char	**tmp;
	char	*test;
	int		i;
	int		j;

	clean = NULL;
	i = -1;
	j = 0;
	while (args && args[++i])
	{
		test = expand_string(ft_strdup(args[i]), status, env);
		if (test && *test)
			j++;
		tmp = ft_realloc((void *)clean, sizeof(char *) * (j + 1));
		if (!tmp)
			return (free_array(args), free_array(clean), NULL);
		clean = tmp;
		if (test && *test)
			clean[j - 1] = ft_strjoin_and_free(clean[j - 1], ft_strdup(args[i]));
			// clean[j - 1] = ft_strjoin_and_free(clean[j - 1],
			// 		expand_string(ft_strdup(args[i]), status, env));
		free(args[i]);
		free(test);
	}
	return (free(args), clean);
}
