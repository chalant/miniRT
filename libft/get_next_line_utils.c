/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/29 15:53:50 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_join(char *s1, char const *buffer, t_read_state rs)
{
	char	*line;
	int		i;

	i = 0;
	line = malloc(sizeof(char) * (rs.cursor + rs.size + 1));
	if (!line)
	{
		free(s1);
		return (NULL);
	}
	while (i < rs.size)
	{
		line[i] = s1[i];
		i++;
	}
	while (rs.start < rs.cursor)
	{
		line[i] = buffer[rs.start];
		i++;
		rs.start++;
	}
	free(s1);
	line[i] = '\0';
	return (line);
}

t_read_state	append(char **line, char *buffer, t_read_state rs)
{
	*line = ft_join(*line, buffer, rs);
	rs.fail = !*line;
	if (rs.fail)
		return (rs);
	rs.size += rs.cursor - rs.start;
	return (rs);
}

t_read_state	next_chunk(int fd, char **line, char *buffer, t_read_state rs)
{
	rs = append(line, buffer, rs);
	rs.bytes_read = read(fd, buffer, BUFFER_SIZE);
	rs.start = 0;
	rs.cursor = 0;
	return (rs);
}
