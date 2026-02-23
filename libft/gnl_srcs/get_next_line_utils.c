/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:01:15 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/24 20:19:42 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_f_free(t_f **flist, int fd)
{
	free(flist[fd]);
	flist[fd] = NULL;
	return (NULL);
}

t_f	*ft_flist_add(t_f **flist, int fd)
{
	t_f	*f;

	f = malloc(sizeof(t_f));
	if (!f)
		return (NULL);
	flist[fd] = f;
	f->fd = fd;
	f->siz_read = read(f->fd, f->buffer, BUFFER_SIZE);
	if (f->siz_read <= 0)
		return (ft_f_free(flist, f->fd));
	f->offset = 0;
	return (f);
}

ssize_t	ft_len(char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strnewjoin(char *str1, t_f *f, ssize_t end)
{
	char	*str2;
	ssize_t	len1;
	char	*new_str;
	ssize_t	i;

	len1 = ft_len(str1);
	str2 = &(f->buffer)[f->offset];
	end -= f->offset;
	new_str = malloc((len1 + end + 1) * sizeof(char));
	if (!new_str)
		return (free(str1), NULL);
	new_str[0] = '\0';
	i = -1;
	while (i++ < len1 - 1)
		new_str[i] = str1[i];
	i = -1;
	while (i++ < end - 1)
		new_str[len1 + i] = str2[i];
	new_str[len1 + i] = '\0';
	free(str1);
	return (new_str);
}
