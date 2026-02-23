/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:34:37 by cghirard          #+#    #+#             */
/*   Updated: 2025/11/22 17:43:42 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

typedef struct s_f
{
	int		fd;
	char	buffer[BUFFER_SIZE];
	ssize_t	siz_read;
	ssize_t	offset;
}	t_f;

void	*ft_f_free(t_f **flist, int fd);
t_f		*ft_flist_add(t_f **flist, int fd);
ssize_t	ft_len(char *str);
char	*ft_strnewjoin(char *str1, t_f *f, ssize_t end);
char	*get_next_line(int fd);

#endif
