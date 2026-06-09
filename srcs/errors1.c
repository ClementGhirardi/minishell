/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 13:53:39 by clement-ghi       #+#    #+#             */
/*   Updated: 2026/06/09 10:03:36 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_file(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	if (file)
		ft_putstr_fd(file, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
}

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
