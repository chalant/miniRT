/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 14:24:32 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/26 11:47:59 by alexphil         ###   ########.fr       */
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
	return (printf("Good map!\n"), 0);
	if (check_scene(&import))
		return (1);
	return (0);
}
