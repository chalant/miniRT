/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:29:20 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/15 22:33:29 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"


int	is_blank(char **line)
{
	char	*tmp;

	tmp = *line;
	if (!tmp)
		return (1);
	if (*tmp == '\n' || *tmp == '\0')
		return (free(*line), 1);
	while (*tmp == ' ')
		tmp++;
	if (*tmp == '\n' || *tmp == '\0')
		return (free(*line), 1);
	else
		return (0);
}

int	expected_infos(t_import *import, char *type)
{
	if (!ft_strcmp(type, "A"))
		return (3);
	else if (!ft_strcmp(type, "C") || !ft_strcmp(type, "L")
		|| !ft_strcmp(type, "li"))
		return (4);
	else if (!ft_strcmp(type, "sp") || !ft_strcmp(type, "pl"))
			return (4 + import->material);
	else if (!ft_strcmp(type, "cy") || !ft_strcmp(type, "cn"))
		return (6 + import->material);
	else if (!ft_strcmp(type, "mt"))
		return (8);
	else
		return (1);
}

int	seen_type(t_import *import, char *type)
{
	if (!ft_strcmp(type, "A"))
		if (++import->ambient && import->ambient > 1)
			return (err("The map has too many ambient elements."));
	else if (!ft_strcmp(type, "C"))
		if (++import->camera && import->camera > 1)
			return (err("The map has too many camera elements."));
	else if (!ft_strcmp(type, "L"))
		if (++import->light && import->light > 1)
			return (err("The map has too many light elements."));
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
		infos = ft_split(line, ' ');
		if (!infos)
			return (free(line), ft_clear_ds(infos), 1);
		if ((int)ft_count_strings(infos) != expected_infos(import, infos[0]))
			return (free(line), ft_clear_ds(infos), 1);
		if (seen_type(import, infos[0]))
			return (free(line), ft_clear_ds(infos), 1);
		if (process_element(import, infos))
			return (free(line), ft_clear_ds(infos), 1);
		ft_clear_ds(infos);
		free(line);
	}
	return (0);
}
