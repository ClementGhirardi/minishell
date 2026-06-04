/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement-ghirardi <clement-ghirardi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 10:39:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/03 13:49:53 by clement-ghi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	existing_path(char *cmd)
{
	if (!access(cmd, F_OK))
		return (1);
	return (0);
}

static char	**get_env_paths(char **envp)
{
	char	**paths;
	int		i;

	if (!envp[0])
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

static void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*tmp;
	char	*full;

	if (!cmd)
		return (NULL);
	if (existing_path(cmd))
		return (ft_strdup(cmd));
	paths = get_env_paths(envp);
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!cmd ||!*cmd)
			return (free(tmp), free_strs(paths), NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(full, X_OK))
			return (free_strs(paths), full);
		free(full);
	}
	return (free_strs(paths), NULL);
}
