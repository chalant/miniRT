/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:58:19 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/26 12:05:13 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	process_cylinder(t_import *import, char **infos)
{
	t_object	cylinder;
	float		diameter;
	float		height;

	if (set_xyz(cylinder.center, infos[1]))
		return (1);
	if (set_normal(cylinder.orientation, infos[2]))
		return (1);
	if (set_unit(&diameter, infos[3]))
		return (1);
	if (set_unit(&height, infos[4]))
		return (1);
	if (set_rgb(cylinder.color, infos[5]))
		return (1);
	if (create_cylinder(&cylinder, height, diameter))
		return (1);
	if (ft_darray_append(&import->minirt->objects, &cylinder))
		return (1);
	return (0);
}

int	process_cone(t_import *import, char **infos)
{
	t_object	cone;
	float		diameter;
	float		height;

	if (set_xyz(cone.center, infos[1]))
		return (1);
	if (set_normal(cone.orientation, infos[2]))
		return (1);
	if (set_unit(&diameter, infos[3]))
		return (1);
	if (set_unit(&height, infos[4]))
		return (1);
	if (set_rgb(cone.color, infos[5]))
		return (1);
	if (create_cone(&cone, height, diameter))
		return (1);
	if (ft_darray_append(&import->minirt->objects, &cone))
		return (1);
	return (0);
}

int	process_element(t_import *import, char **infos)
{
	if (!ft_strcmp(infos[0], "A"))
		return (process_ambient(import, infos));
	else if (!ft_strcmp(infos[0], "C"))
		return (process_camera(import, infos));
	else if (!ft_strcmp(infos[0], "L") || !ft_strcmp(infos[0], "li"))
		return (process_light(import, infos));
	else if (!ft_strcmp(infos[0], "sp"))
		return (process_sphere(import, infos));
	else if (!ft_strcmp(infos[0], "pl"))
		return (process_plane(import, infos));
	else if (!ft_strcmp(infos[0], "cy"))
		return (process_cylinder(import, infos));
	else if (!ft_strcmp(infos[0], "cn"))
		return (process_cone(import, infos));
	return (1);
}
