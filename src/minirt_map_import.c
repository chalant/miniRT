/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_import.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 14:24:32 by alexphil          #+#    #+#             */
/*   Updated: 2024/01/31 13:25:39 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include <fcntl.h>

typedef	struct	s_import
{
	t_minirt	*minirt;
	int			fd;
	int			elements;
	int			ambient;
	int			camera;
	int			light;
}	t_import;

int	check_extension(char *filename, char *extension)
{
	int	i;
	int	f_len;
	int	e_len;

	if (!filename || !extension || filename[0] == '.')
		return (perror("Error: check_extension() misuse."), 1);
	i = 0;
	f_len = ft_strlen(filename);
	e_len = ft_strlen(extension);
	while (filename[i] != '.')
		i++;
	if (!ft_strncmp(&filename[i], extension, e_len) && i + e_len == f_len)
		return (0);
	return (1);
}

int	init_import(t_import *import, t_minirt *minirt, char *map)
{
	import->minirt = minirt;
	import->fd = open(map, O_RDONLY);
	if (import->fd == -1)
		return (perror("Error: Open() failure."), 1);
	import->elements = 0;
	import->ambient = 0;
	import->camera = 0;
	import->light = 0;
	return (0);
}

int	is_blank(char *line)
{
	if (!line || *line == '\n' || *line == '\0')
		return (free(line), 1);
	while (*line == ' ')
		line++;
	if (*line == '\n' || *line == '\0')
		return (free(line), 1);
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
	else if (!ft_strcmp(type, "cy"))
		return (6);
	else
		return (-1);
}

int	seen_type(t_import *import, char **line)
{
	char	*type;

	type = line[0];
	if (ft_strcmp(type, "A"))
		if (++import->ambient && import->ambient > 1)
			return (1);
	if (ft_strcmp(type, "C"))
		if (++import->camera && import->camera > 1)
			return (1);
	if (ft_strcmp(type, "L"))
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
		if (is_blank(line))
			continue ;
		else
			import->elements++;
		infos = ft_split(line, ' ');
		if (!infos)
			return (free(line), ft_clear_ds(infos), 1);
		if ((int)ft_count_strings(infos) != expected_infos(infos[0]))
			return (free(line), ft_clear_ds(infos), 1);
		if (seen_type(import, infos))
			return (free(line), ft_clear_ds(infos), 1);
		// if (spawn_element(import, infos)) // TODO: spawn_element()
		// 	return (free(line), ft_clear_ds(infos), destroy_elements(import)); // TODO: destroy_elements()
		ft_clear_ds(infos);
		free(line);
	}
	return (0);
}

int	check_scene(t_import *import)
{
	if (!import->A || !import->C || import->L)
		return (1);
	return (0);
}

int	import_map(t_minirt *minirt, char **av)
{
	t_import	import;

	if (check_extension(av[1], ".rt"))
		return (1);
	if (init_import(&import, minirt, av[1]))
		return (1);
	if (read_map(&import))
		return (1);
	if (check_scene(&import))
		return (1);
	// if (spawn_element(import, infos)) // TODO: spawn_element()
		// 	return (free(line), ft_clear_ds(infos), destroy_elements(import)); // TODO: destroy_elements()
	return (0);
}

// PARSING & MAP IMPORT:
// [X] Check if input is a .rt file and check that it's not JUST ".rt"
// [X] Read the map line per line with GNL and count the number of elements
// [X] Split each line against blanks and check if it's n-sized as expected PER type
// [X] If A, C or L type, check if already seen in the map (can be declared only once)
// [ ] If map is correct, process each element data into memory for rendering (where and how, see w/ Yves)

// COMPILATION:
// [ ] Incorporate GNL and ft_printf into Yves's Libft Makefile for compilation

// Expected elements per type:
// A: 3
// C: 4
// L: 4
// sp: 4
// pl: 4
// cy: 6