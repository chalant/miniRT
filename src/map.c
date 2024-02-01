/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 14:24:32 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/01 15:37:19 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

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
