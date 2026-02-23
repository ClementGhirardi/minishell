/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:34:13 by cghirard          #+#    #+#             */
/*   Updated: 2026/01/14 15:54:23 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_read_again(t_f **flist, t_f *f, char *next_line, ssize_t i)
{
	next_line = ft_strnewjoin(next_line, f, i);
	if (!next_line)
		return (ft_f_free(flist, f->fd));
	f->offset = 0;
	f->siz_read = read(f->fd, f->buffer, BUFFER_SIZE);
	if (f->siz_read == -1)
	{
		free(next_line);
		return (ft_f_free(flist, f->fd));
	}
	return (next_line);
}

char	*ft_last_line(t_f **flist, t_f *f, char *next_line)
{
	if (!next_line || next_line[0] == '\0')
	{
		free(next_line);
		return (ft_f_free(flist, f->fd));
	}
	ft_f_free(flist, f->fd);
	return (next_line);
}

char	*ft_mk_next_line(char **next_line)
{
	*next_line = malloc(sizeof(char));
	if (!(*next_line))
		return (NULL);
	(*next_line)[0] = '\0';
	return (*next_line);
}

char	*ft_next_line(t_f **flist, t_f *f)
{
	char	*next_line;
	ssize_t	i;

	if (!ft_mk_next_line(&next_line))
		return (ft_f_free(flist, f->fd));
	while (f->siz_read)
	{
		i = f->offset;
		while (i < f->siz_read)
		{
			if (f->buffer[i] == '\n')
			{
				next_line = ft_strnewjoin(next_line, f, i + 1);
				if (!next_line)
					return (ft_f_free(flist, f->fd));
				f->offset = i + 1;
				return (next_line);
			}
			i++;
		}
		next_line = ft_read_again(flist, f, next_line, i);
		if (!next_line)
			return (NULL);
	}
	return (ft_last_line(flist, f, next_line));
}

char	*get_next_line(int fd)
{
	static t_f	*flist[1024];
	t_f			*f;
	char		*next_line;

	if (fd < 0 || 1023 < fd || BUFFER_SIZE <= 0)
		return (NULL);
	if (!flist[fd])
	{
		f = ft_flist_add(flist, fd);
		if (!f)
			return (NULL);
	}
	else
		f = flist[fd];
	next_line = ft_next_line(flist, f);
	if (fd == 0)
		ft_f_free(flist, f->fd);
	return (next_line);
}
