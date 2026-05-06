#include "../includes/minishell.h"

int	dollar_finder(char *file)
{
	int	i;

	i = 0;
	while (file[i])
	{
		if (file[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

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

// int	odd_squotes(char *file)
// {
// 	int	i;

// 	i = 0;
// 	while (file[i] == '\'')
// 		i++;
// 	return (i % 2);
// }

char	*squote_exception(char *file, int *i)
{
	int	start;

	if (!file || !*file)
		return (NULL);
	if (file[*i] == '\'' && file[*i + 1] == '$')
	{
		// if (!odd_squotes(&file[*i]))
		// 	return (NULL);
		while (file[*i] == '\'' && file[*i + 1] == '\'')
			(*i)++;
		if (file[*i + 1] == '$')
		{
			start = ++(*i);
			while (file[*i] && file[*i] != '\'')
				(*i)++;
			if (file[*i])
				(*i)++;
			return (ft_substr(file, start, *i - 1 - start));
		}
	}
	return (NULL);
}

char	*expand_dollar_in_filename(char *file, char **env)
{
	char	*filename;
	char	*tmp;
	int		i;
	int		start;

	i = 0;
	filename = NULL;
	while (file[i])
	{
		filename = ft_strjoin_and_free(filename, squote_exception(file, &i));
		start = i;
		if (file[i] == '$' || file[i] == '\'' || file[i] == '"')
			i++;
		while (file[i] && file[i] != '$' && file[i] != '\'' && file[i] != '"')
			i++;
		tmp = ft_substr(file, start, i - start);
		if (!tmp)
		{
			free(file);
			return (NULL);
		}
		tmp = expand_string(tmp, env);
		filename = ft_strjoin_and_free(filename, tmp);
	}
	free(file);
	return (filename);
}

char	**alloc_array(char **array, char **env)
{
	char	**new_args;
	char	*tmp;
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (array[i])
	{
		tmp = expand_dollar_in_filename(ft_strdup(array[i]), env);
		if (tmp)
			count++;
		free(tmp);
		i++;
	}
	new_args = malloc(sizeof(char *) * (count + 1));
	if (!new_args)
		return (NULL);
	return (new_args);
}

char	**filter_and_dup_array(char **array, char **env)
{
	char	**new;
	char	*tmp;
	int		i;
	int		j;

	i = -1;
	j = -1;
	new = alloc_array(array, env);
	if (!new)
		return (NULL);
	while (array[++i])
	{
		tmp = expand_dollar_in_filename(array[i], env);
		if (tmp)
			new[++j] = tmp;
	}
	new[++j] = NULL;
	free(array);
	return (new);
}
