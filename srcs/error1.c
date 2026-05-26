/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error1.c.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:22:58 by cghirard          #+#    #+#             */
/*   Updated: 2026/05/26 14:23:17 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*replace(char *new, char *command)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	if (!new || !command)
		return (NULL);
	new[++j] = '$';
	new[++j] = '\'';
	while (command[++i])
	{
		if (command[i] == '\n')
		{
			new[++j] = '\\';
			new[++j] = 'n';
		}
		else
			new[++j] = command[i];
	}
	new[++j] = '\'';
	new[++j] = '\0';
	return (new);
}

static int	count_occurrences(char *s, char c)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == c)
			i++;
		i++;
	}
	return (i);
}

static char	*newline_replace(char *command)
{
	int		i;
	char	*new;

	if (!command)
		return (NULL);
	i = count_occurrences(command, '\n');
	if (!i)
		return (ft_strdup(command));
	new = malloc(sizeof(char) * (i + 4));
	if (!new)
		return (NULL);
	return (replace(new, command));
}

void	error_command(char *command)
{
	char	*new_command;

	if (command && ft_is_in('\n', command))
		new_command = newline_replace(command);
	else
		new_command = ft_strdup(command);
	ft_putstr_fd("minishell: ", 2);
	if (new_command)
		ft_putstr_fd(new_command, 2);
	ft_putendl_fd(": command not found", 2);
	free(new_command);
}

int	error_open(char *file)
{
	if (!file)
		return (ft_putendl_fd("minishell: : No such file or directory", 2), 1);
	if (file[0] == '$')
		return (1);
	if (access(file, F_OK))
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(file, 2),
			ft_putendl_fd(": No such file or directory", 2), 1);
	if (access(file, X_OK))
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(file, 2),
			ft_putendl_fd(": Permission denied", 2), 1);
	return (ft_putstr_fd("minishell: ", 2),
		ft_putendl_fd(": open failed", 2), 1);
}
