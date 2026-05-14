#include "../includes/minishell.h"

// void	*ft_realloc(void *ptr, size_t size)
// {
// 	unsigned char	*new;
// 	unsigned char	*p;
// 	size_t			i;

// 	if (!size)
// 		return (free(ptr), NULL);
// 	p = ptr;
// 	i = 0;
// 	new = ft_calloc(1, size);
// 	if (!new)
// 		return (NULL);
// 	while (p && i < size)
// 	{
// 		new[i] = p[i];
// 		i++;
// 	}
// 	free(ptr);
// 	return (new);
// }

// char	*ft_strjoin_sep_realloc(char **array, char c)
// {
// 	char	*str;
// 	int		i;
// 	int		j;
// 	int		len_str;
// 	int		len;

// 	str = NULL;
// 	i = 0;
// 	while (array[i])
// 	{
// 		j = 0;
// 		len_str = ft_safe_strlen(str);
// 		len = ft_safe_strlen(array[i]) + len_str;
// 		str = ft_realloc(str, sizeof(char) * (len + 2));
// 		if (!str)
// 			return (NULL);
// 		while (j < len_str)
// 			j++;
// 		while (*array[i])
// 			str[j++] = *array[i]++;
// 		str[j] = c;
// 		i++;
// 	}
// 	return (str);
// }

// char	*expand_string_heredoc(char *str, char **env)
// {
// 	int		i;
// 	char	*result;
// 	char	*var;

// 	i = 0;
// 	result = ft_strdup("");
// 	if (!result)
// 		return (free(str), NULL);
// 	while (str && str[i])
// 	{
// 		if (str[i] == '$')
// 			var = extract_var_name(str, &i, env);
// 		else
// 			var = ft_substr(str, i++, 1);
// 		result = ft_strjoin_and_free(result, var);
// 	}
// 	return (free(str), result);
// }

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (300);
	while (s1[i] && s2[i])
	{
		if (s1[i] - s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}
