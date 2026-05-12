// #include "../includes/minishell.h"

// char	*split_arg(char *arg, int *i)
// {
// 	int	start;
// 	int	stop;

// 	if (!arg || !dollar_finder(arg))
// 		return (arg);
// 	while ((arg[*i] && (arg[*i] == '\'' || arg[*i] == '"'))
// 		&& (arg[*i + 1] == '\'' || arg[*i + 1] == '"'))
// 		(*i)++;
// 	start = *i;
// 	stop = '\0';
// 	if (arg[*i] == '$')
// 		(*i)++;
// 	else if (arg[*i] == '\'' || arg[*i] == '"')
// 		stop = *i;
// 	while (arg[*i] && arg[*i] != '$' && arg[*i] != stop)
// 		(*i)++;
// 	return (ft_substr(arg, start, *i - start));
// }

// int	existing_var(char *arg, char **env)
// {
// 	char	*tmp;
// 	char	*var;
// 	int		i;

// 	if (!arg)
// 		return (0);
// 	i = 0;
// 	while (arg[i])
// 	{
// 		var = quotes_cleaner(ft_strdup(arg));
// 		tmp = split_arg(var, &i); //arg, pas var
// 		// if (!dollar_finder(arg))
// 		// 	return (1);
// 		// var = quotes_cleaner(ft_strdup(arg));
// 		tmp = expand_var(var, env);
// 		if (!tmp)
// 			return (free(var), 0);
// 		free(var);
// 		free(tmp);
// 	}
// 	return (1);
// }

// char	*expand_var(char *str, char **env)
// {
// 	char	*var;
// 	char	*tmp;
// 	int		i;

// 	var = NULL;
// 	i = 0;
// 	while (str && str[i])
// 	{
// 		if (str[i] == '$')
// 			tmp = extract_var_name(str, &i, env);
// 		else
// 			tmp = ft_substr(str, i++, 1);
// 		if (!tmp && !var && !str[i])
// 			return (NULL);
// 		if (!tmp)
// 			tmp = ft_strdup("");
// 		var = ft_strjoin_and_free(var, tmp);
// 		if (!var)
// 			return (NULL);
// 	}
// 	return (var);
// }

// char	*expand_file(char *file, char **env)
// {
// 	int	i;

// 	i = -1;
// 	if (file && file[0] != '\'' && file[0] != '"') // checker que $
// 	{
// 		if (!existing_var(file, env))
// 			return (error_file(file), NULL);
// 	}
// 	if (file && file[i] == '\'')
// 		file = quotes_cleaner(file);
// 	else if (file && file[i] == '"')
// 	{
// 		file = quotes_cleaner(file);
// 		file = expand_var(file, env);
// 	}
// 	else
// 		file = expand_var(file, env);
// 	return (file);
// }

// char	**expand_args(char **args, char **env)
// {
// 	char	*tmp;
// 	int		i;
// 	char	quote;

// 	i = -1;
// 	ft_printf("args[0] debut expanargs = %s\n", args[0]);
// 	args = filter_args(args, env);
// 	ft_printf("args[0] apres filterargs= %s\n", args[0]);
// 	while (args && args[++i])
// 		args[i] = skip_empty_quotes(args[i]);
// 	quote = '\0';
// 	if (args && *args)
// 		quote = **args;
// 	i = -1;
// 	//if (args && *args && (quote == '\'' || quote == '"'))
// 	//{
// 		while (args && args[++i])
// 			args[i] = quotes_cleaner(args[i]);
// 	//}
// 	i = 0;
// 	ft_printf("quote = %c\n", quote);
// 	ft_printf("args[0] == %s\n", args[0]);
// 	while (args && args[i] && *args[i]
// 		&& (quote != '\'' || *args[i] != '$'))
// 	{
// 		tmp = expand_var(args[i], env);
// 		if (tmp)
// 			args[i] = tmp;
// 		else
// 			break ;
// 		i++;
// 	}
// 	return (args);
// }

// void	expander(t_ast *node, char **env)
// {
// 	if (node && node->args)
// 		node->args = expand_args(node->args, env);
// 	if (node && node->file)
// 		node->file = expand_file(node->file, env);
// }
