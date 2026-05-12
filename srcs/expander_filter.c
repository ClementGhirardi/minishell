// #include "../includes/minishell.h"

// char	*filter_file(char *file, char **env)
// {
// 	if (file && file[0] != '\'' && file[0] != '"')
// 	{
// 		if (!existing_var(file, env))
// 			return (NULL);
// 	}
// 	return (file);
// }

// char	**fill_new_args(char **args, int count, char **env)
// {
// 	char	**new_args;
// 	int		i;
// 	int		j;

// 	new_args = malloc(sizeof(char *) * (count + 1));
// 	if (!new_args)
// 		return (NULL);
// 	i = -1;
// 	j = -1;
// 	while (args && args[++i])
// 	{
// 		if (existing_var(args[i], env))
// 			new_args[++j] = ft_strdup(args[i]);
// 	}
// 	new_args[++j] = NULL;
// 	return (new_args);
// }

// char	**filter_args(char **args, char **env)
// {
// 	char	**new_args;
// 	int		i;
// 	int		count;

// 	i = -1;
// 	count = 0;
// 	while (args && args[++i])
// 	{
// 		if (existing_var(args[i], env))
// 			count++;
// 	}
// 	ft_printf("count = %d\n", count);
// 	new_args = fill_new_args(args, count, env);
// 	free_array(args);
// 	return (new_args);
// }
