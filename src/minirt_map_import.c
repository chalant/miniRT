/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_import.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 14:24:32 by alexphil          #+#    #+#             */
/*   Updated: 2024/01/31 16:36:05 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include <fcntl.h>

typedef	struct	s_import
{
	t_minirt	*minirt;
	int			fd;
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
	import->ambient = 0;
	import->camera = 0;
	import->light = 0;
	return (0);
}

int	is_blank(char *line)
{
	if (!line)
		return (1);
	if (*line == '\n' || *line == '\0')
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
	else if (!ft_strcmp(type, "cy") || !ft_strcmp(type, "cn"))
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

enum	e_ranges
{
	LIGHT,
	RGB,
	NORMAL,
	FOV,
}	t_ranges;

int	check_range(float value, int type) // Extend responsabilities to aleviate the processing functions (tokenize, verify size..)
{
	if (type == LIGHT && value >= 0.0 && value <= 1.0)
		return (0);
	if (type == RGB && (int)value >= 0 && (int)value <= 255)
		return (0);
	if (type == NORMAL && value >= -1.0 && value <= 1.0)
		return (0);
	if (type == FOV && (int)value >= 0 && (int)value <= 180)
		return (0);
	else
		return (1);
}

int	process_ambient(t_import *import, char **infos)
{
	t_light		ambient;
	char		**colors;
	int			i;

	colors = ft_split(infos[2], ',');
	if (!colors)
		return (1);
	if (ft_lstsize(colors) != 3)
		return (1);
	i = -1;
	while (++i < 3)
		if (check_range(ft_atoi(colors[i]), RGB))
			return (ft_clear_ds(colors), 1);
	ambient.color[0] = ft_atoi(colors[0]);
	ambient.color[1] = ft_atoi(colors[1]);
	ambient.color[2] = ft_atoi(colors[2]);
	ambient.color[3] = 0;
	import->minirt->ambient = ambient;
	ft_clear_ds(colors);
	return (0);
}

// int	process_camera(t_import *import, char **infos)
// {
// 	t_camera	camera;

// 	vector_magnitude()
// }

// int	process_light(t_import *import, char **infos)
// {
// 	t_object	obj;
// 	t_light		light;
// }

// int	process_sphere(t_import *import, char **infos)
// {
// 	t_object	obj;
// 	t_sphere	sphere;
// }

// int	process_plane(t_import *import, char **infos)
// {
// 	t_object	obj;
// 	t_plane		plane;
// }

// int	process_cylinder(t_import *import, char **infos)
// {
// 	t_object	obj;
// 	t_cylinsder	cylinder;
// }

// int	process_cone(t_import *import, char **infos)	TODO: T_CONE STRUCT
// {
// 	t_object	obj;
// 	t_cone		cone;
// }

// [ ] TODO: Manage array deletion if an error occur
int	process_element(t_import *import, char **infos)
{
	if (ft_strcmp(infos[0], "A"))
		return (process_ambient(import, infos));
	if (ft_strcmp(infos[0], "C"))
		process_camera(import, infos);
	// if (ft_strcmp(infos[0], "L"))
	// 	process_light(import, infos);
	// if (ft_strcmp(infos[0], "sp"))
	// 	process_sphere(import, infos);
	// if (ft_strcmp(infos[0], "pl"))
	// 	process_plane(import, infos);
	// if (ft_strcmp(infos[0], "cy"))
	// 	process_cylinder(import, infos);
	// if (ft_strcmp(infos[0], "cn"))
	// 	process_cone(import, infos);
	else
		return (1); // ft_darray_delete if something already exits, othwewise just ret 1
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

int	check_scene(t_import *import)
{
	if (!import->ambient || !import->camera || import->light)
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
	return (0);
}

// PARSING & MAP IMPORT:
// [X] Check if input is a .rt file and check that it's not JUST ".rt"
// [X] Read the map line per line with GNL and count the number of elements
// [X] Split each line against blanks and check if it's n-sized as expected PER type
// [X] If A, C or L type, check if already seen in the map (can be declared only once)
// [ ] If map is correct, process each element data into memory for rendering
// [ ] Verify that each part of the informations from an element is correct per the subject specifications (RGB, Normalization, Light Range..)
// [ ] Manage floats with either custom atoi or by splitting and calculing 

// COMPILATION:
// [X] Incorporate GNL and ft_printf into Yves's Libft Makefile for compilation

// Object types and expected elements per type:
// A: 3
// C: 4
// L: 4
// sp: 4
// pl: 4
// cn: 5
// cy: 6

// Prototyping 
// int get_element(t_import *import, char **infos)
// {
// 	t_object	new;
// 	t_sphere	*sphere;

	
// 	if (ft_strcmp("sp", infos[0]))
// 	{
// 		sphere->radius;
// 	}
// 	new.shape = sphere;
// 	if (ft_darray_append(import->minirt->objects, &new))
// }
