/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/29 18:00:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_read_state	reset_state(t_read_state rs)
{
	rs.bytes_read = -1;
	rs.cursor = 0;
	rs.size = 0;
	rs.eof = 0;
	rs.start = 0;
	rs.fail = 0;
	return (rs);
}

t_read_state	load_line(int fd, t_read_state rs, char **line, char *buffer)
{
	while (rs.bytes_read)
	{
		rs.start = rs.cursor;
		while (rs.cursor < rs.bytes_read && buffer[rs.cursor] != '\n')
			rs.cursor++;
		if (rs.cursor >= rs.bytes_read)
		{
			rs = next_chunk(fd, line, buffer, rs);
			if (rs.fail || rs.bytes_read == -1 || rs.eof)
				return (rs);
		}
		else if (buffer[rs.cursor] == '\n')
		{
			rs.cursor++;
			return (append(line, buffer, rs));
		}
	}
	return (rs);
}

t_read_state	read_next_line(int fd, char **line, t_read_state rs)
{
	static char	buffer[BUFFER_SIZE];

	if (rs.cursor >= rs.bytes_read)
	{
		rs.bytes_read = read(fd, buffer, BUFFER_SIZE);
		rs.cursor = 0;
	}
	if (rs.bytes_read == -1 || rs.bytes_read == 0)
	{
		rs.eof = 1;
		return (rs);
	}
	return (load_line(fd, rs, line, buffer));
}

char	*get_next_line(int fd)
{
	char				*line;
	char				*tmp;
	static t_read_state	rs = {-1, 0, 0, 0, 0, 0, 0};

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	line = malloc(sizeof(char));
	if (!line)
		return (NULL);
	rs = read_next_line(fd, &line, rs);
	if (rs.fail || rs.bytes_read == -1 || rs.eof)
	{
		rs = reset_state(rs);
		free(line);
		return (NULL);
	}
	tmp = ft_strndup(line, rs.size);
	free(line);
	rs.start = 0;
	rs.size = 0;
	if (rs.bytes_read == 0)
		rs.eof = 1;
	return (tmp);
}
