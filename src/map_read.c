/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:29:20 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/07 13:21:06 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"


int	is_blank(char **line)
{
	if (!*line || !*line[0])
		return (1);
	if (**line == '\n' || **line == '\0')
		return (free(*line), 1);
	while (**line == ' ')
		(*line)++;
	if (**line == '\n' || **line == '\0')
		return (free(*line), 1);
	else
		return (0);
}

int	expected_infos(char *type)
{
	if (!ft_strcmp(type, "A"))
		return (3);
	else if (!ft_strcmp(type, "C") || !ft_strcmp(type, "L")
		|| !ft_strcmp(type, "sp") || !ft_strcmp(type, "pl"))
		return (4);
	else if (!ft_strcmp(type, "cy") || !ft_strcmp(type, "cn"))
		return (6);
	else
		return (-1);
}

int	seen_type(t_import *import, char **line)
{
	char	*type;

	type = line[0];
	if (!ft_strcmp(type, "A"))
		if (++import->ambient && import->ambient > 1)
			return (1);
	if (!ft_strcmp(type, "C"))
		if (++import->camera && import->camera > 1)
			return (1);
	if (!ft_strcmp(type, "L"))
		if (++import->light && import->light > 1)
			return (1);
	return (0);
}

int	read_map(t_import *import)
{
	char	*line;
	char	**infos;

	while (1)
	{
		line = get_next_line(import->fd);
		if (!line)
			break ;
		if (is_blank(&line))
			continue ;
		// printf("I should be printed once\n");
		infos = ft_split(line, ' ');
		if (!infos)
			return (free(line), ft_clear_ds(infos), 1);
		if ((int)ft_count_strings(infos) != expected_infos(infos[0]))
			return (free(line), ft_clear_ds(infos), 1);
		if (seen_type(import, infos))
			return (free(line), ft_clear_ds(infos), 1);
		if (process_element(import, infos))
			return (free(line), ft_clear_ds(infos), 1);
		ft_clear_ds(infos);
		free(line);
	}
	return (0);
}
