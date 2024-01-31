/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:54:53 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/31 12:36:44 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

// # include "ft_printf.h"
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# if BUFFER_SIZE > 1000000
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_read_state
{
	int			bytes_read;
	int			cursor;
	int			start;
	int			size;
	int			fail;
	int			eof;
	int			t_size;
}				t_read_state;

char			*get_next_line(int fd);
char			*ft_join(char *s1, char const *buffer, t_read_state rs);

t_read_state	append(char **line, char *buffer, t_read_state rs);
t_read_state	next_chunk(int fd, char **line, char *buffer, t_read_state rs);

#endif
