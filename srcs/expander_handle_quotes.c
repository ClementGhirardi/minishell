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

// // static char	*skip_empty_last_quotes(char *str)
// // {
// // 	char	*clean;
// // 	int		len;
// // 	char	quote;

// // 	if (!str)
// // 		return (NULL);
// // 	len = ft_strlen(str);
// // 	if (str[--len] != '"' && str[len] != '\'')
// // 		return (str);
// // 	quote = str[len];
// // 	while (len > 0 && str[len] == quote && str[len - 1] == quote)
// // 	{
// // 		len -= 2;
// // 		if (str[len] == '\'' || str[len] == '"')
// // 			quote = str[len];
// // 		else
// // 			break ;
// // 	}
// // 	clean = ft_substr(str, 0, len + 1);
// // 	free(str);
// // 	return (clean);
// // }

// // char	*skip_empty_first_quotes(char *str)
// // {
// // 	char	*clean;
// // 	int		i;
// // 	int		start;
// // 	char	quote;

// // 	if (!str || (*str != '"' && *str != '\''))
// // 	{
// // 		clean = ft_strdup(str);
// // 		free(str);
// // 		return (clean);
// // 	}
// // 	i = 0;
// // 	quote = *str;
// // 	while (str[i] && str[i + 1] && str[i] == quote && str[i + 1] == quote)
// // 	{
// // 		i += 2;
// // 		if (str[i] == '\'' || str[i] == '"')
// // 			quote = str[i];
// // 		else
// // 			break ;
// // 	}
// // 	start = i;
// // 	while (str[i])
// // 		i++;
// // 	clean = ft_substr(str, start, i - start);
// // 	clean = skip_empty_last_quotes(clean);
// // 	return (free(str), clean);
// // }

// char	*skip_empty_quotes(char *str)
// {
// 	char	*clean;
// 	int		i;

// 	i = 0;
// 	clean = NULL;
// 	while (str && str[i])
// 	{
// 		if ((str[i] == '\'' || str[i] == '"')
// 			&& str[i + 1] && str[i + 1] == str[i])
// 			i++;
// 		else
// 		{
// 			clean = ft_strjoin_and_free(clean, ft_substr(str, i, 1));
// 			if (!clean)
// 				return (free(str), NULL);
// 		}
// 		i++;
// 	}
// 	return (clean);
// }

// char	*quotes_cleaner(char *str)
// {
// 	char	*clean;
// 	char	*tmp;
// 	int		i;
// 	int		start;
// 	char	quote;

// 	clean = NULL;
// 	str = skip_empty_quotes(str);
// 	i = 0;
// 	while (str && str[i] && (str[i] == '\'' || str[i] == '"'))
// 	{
// 		quote = str[i];
// 		start = ++i;
// 		while (str[i] && str[i] != quote)
// 			i++;
// 		clean = ft_strjoin_and_free(clean, ft_substr(str, start, i - start));
// 		if (!clean)
// 			return (free(str), NULL);
// 		i++;
// 	}
// 	while (str && str[i] && str[i] != '\'' && str[i] != '"')
// 		clean = ft_strjoin_and_free(clean, ft_substr(str, i++, 1));
// 	if (str[i])
// 	{
// 		tmp = quotes_cleaner(&str[i]);
// 		clean = ft_strjoin_and_free(clean, tmp);
// 	}
// 	return (free(str), clean);
// }
