/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 14:24:32 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/12 16:48:10 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	check_extension(char *filename, char *extension)
{
	int	i;
	int	f_len;
	int	e_len;

	if (filename[0] == '.' && filename[1] != '/')
		return (err("Wrong map extension (Expected *.rt file)."));
	if (!ft_strncmp(filename, "./", 2))
		filename += 2;
	i = 0;
	f_len = ft_strlen(filename);
	e_len = ft_strlen(extension);
	while (filename[i] != '.')
		i++;
	if (!ft_strncmp(&filename[i], extension, e_len) && i + e_len == f_len)
		return (0);
	return (err("Wrong map extension: Expecting *.rt file)."));
}

int	init_import(t_import *import, t_minirt *minirt, char *map)
{
	if (ft_darray_init(&minirt->objects, sizeof(t_object), 1))
		return (1);
	import->minirt = minirt;
	import->fd = open(map, O_RDONLY);
	if (import->fd == -1)
		return (err("Open() failure."));
	import->ambient = 0;
	import->camera = 0;
	import->light = 0;
	return (0);
}

int	check_scene(t_import *import)
{
	if (!import->ambient || !import->camera || import->light)
		return (err("Scene needs one of each: Ambient, Camera and Light."));
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
	else
		return (printf("Good map!\n"), 0);
	// if (check_scene(&import))
	// 	return (1);
	return (0);
}

// PARSING & MAP IMPORT:
// [X] Check if input is a .rt file and check that it's not JUST ".rt"
// [X] Read the map line per line with GNL and count the number of elements
// [X] Split each line against blanks and check if it's n-sized as expected PER type
// [X] If A, C or L type, check if already seen in the map (can be declared only once)
// [WIP] If map is correct, process each element data into memory for rendering
// 	[X] Verify that each part of the informations from an element is correct per the subject specifications (RGB, Normalization, Light Range..)
// 	[X] Manage floats with a home made atof + Manage negatives float too!
// 	[X] Process Ambient
//  [X] Process Camera
//  [X] Process Light
//  [X] Process Sphere
//  [X] Process Plane
//  [X] Process Cylinder
//  [X] Process Cone
// 	[ ] TODO: Manage array deletion if an error occur

// atof
// [X] Should atof supports numbers with 2 decimals ? Well now it does.