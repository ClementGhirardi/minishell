// #include "../includes/minishell.h"

// int	dollar_finder(char *file)
// {
// 	int	i;

// 	i = 0;
// 	while (file[i])
// 	{
// 		if (file[i] == '$')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

// int	is_valid_variable(char *arg, char **env)
// {
// 	char	*tmp;
// 	char	*arg_copy;

// 	arg_copy = ft_strdup(arg);
// 	if (!arg)
// 		return (0);
// 	tmp = expand_string(arg_copy, env);
// 	if (!tmp)
// 		return (0);
// 	free(tmp);
// 	return (1); 
// }

// char	*skip_empty_quotes(char *file)
// {
// 	char	*clean;
// 	int		i;
// 	int		start;
// 	char	quote;

// 	i = 0;
// 	while (file[i] && (file[i] == '\'' || file[i] == '"'))
// 	{
// 		quote = file[i];
// 		if (!file[i + 1] || file[i + 1] != quote)
// 		// if (file[i + 1] != '\'' && file[i + 1] != '"')
// 		// 	break ;
// 		// if (file[i] == file[i + 1])
// 		// 	i++;
// 		i++;
// 	}
// 	if ((i % 2))
// 		i++;
// 	start = i;
// 	while (file[i])
// 		i++;
// 	clean = ft_substr(file, start, i - (start * 2));
// 	free(file);
// 	return (clean);
// }

// // char	*squote_exception(char *file)
// // {
// // 	int	i;
// // 	int	start;

// // 	if (!file || !*file)
// // 		return (NULL);
// // 	i = 0;
// // 	ft_printf("file avant squote exception = %s\n", file);
// // 	//file = skip_empty_quotes(file);
// // 	if (file[i] == '\'' && file[i + 1] == '$')
// // 	{
// // 		while (file[i] == '\'' && file[i + 1] == '\'')
// // 			i++;
// // 		if (file[i + 1] == '$')
// // 		{
// // 			start = ++i;
// // 			while (file[i] && file[i] != '\'')
// // 				i++;
// // 			if (file[i])
// // 				i++;
// // 			return (ft_substr(file, start, i - 1 - start));
// // 		}
// // 	}
// // 	return (NULL);
// // }

// char	*expand_dollar_in_filename(char *file, char **env)
// {
// 	char	*filename;
// 	char	*tmp;
// 	int		i;
// 	int		start;

// 	i = 0;
// 	filename = NULL;
// 	file = ft_strjoin_and_free(filename, skip_empty_quotes(file));
// 	if (file && file[0] == '\'' && file[1] == '$')
// 	{
// 		tmp = ft_substr(file, 1, ft_strlen(file) - 2);
// 		free(file);
// 		return (tmp);
// 	}
// 	while (file[i])
// 	{
// 		start = i;
// 		if (file[i] == '$' || file[i] == '\'' || file[i] == '"')
// 			i++;
// 		while (file[i] && file[i] != '$' && file[i] != '\'' && file[i] != '"')
// 			i++;
// 		tmp = ft_substr(file, start, i - start);
// 		if (!tmp)
// 			return (free(file), NULL);
// 		filename = ft_strjoin_and_free(filename, expand_string(tmp, env));
// 	}
// 	return (free(file), filename);
// }

// char	**alloc_array(char **array, char **env)
// {
// 	char	**new_args;
// 	char	*tmp;
// 	int		i;
// 	int		count;

// 	i = 0;
// 	count = 0;
// 	while (array[i])
// 	{
// 		tmp = expand_dollar_in_filename(ft_strdup(array[i]), env);
// 		if (tmp)
// 			count++;
// 		free(tmp);
// 		i++;
// 	}
// 	new_args = malloc(sizeof(char *) * (count + 1));
// 	if (!new_args)
// 		return (NULL);
// 	return (new_args);
// }

// char	**filter_and_dup_array(char **array, char **env)
// {
// 	char	**new;
// 	char	*tmp;
// 	int		i;
// 	int		j;

// 	i = -1;
// 	j = -1;
// 	new = alloc_array(array, env);
// 	if (!new)
// 		return (NULL);
// 	while (array[++i])
// 	{
// 		tmp = expand_dollar_in_filename(array[i], env);
// 		if (tmp)
// 			new[++j] = tmp;
// 	}
// 	new[++j] = NULL;
// 	free(array);
// 	return (new);
// }
