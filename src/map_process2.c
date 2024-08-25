/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:52:29 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/26 11:59:16 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	process_ambient(t_import *import, char **infos)
{
	t_light		ambient;

	if (set_light(&ambient.brightness, infos[1]))
		return (1);
	if (set_rgb(ambient.color, infos[2]))
		return (1);
	import->minirt->ambient = ambient;
	return (0);
}

int	process_camera(t_import *import, char **infos)
{
	if (set_xyz(import->minirt->camera.origin, infos[1]))
		return (1);
	if (set_normal(import->minirt->camera.orientation, infos[2]))
		return (1);
	if (set_fov(&import->minirt->camera.fov, infos[3]))
		return (1);
	return (0);
}

int	process_light(t_import *import, char **infos)
{
	t_light	light;

	if (set_xyz(light.position, infos[1]))
		return (1);
	if (set_light(&light.brightness, infos[2]))
		return (1);
	if (set_rgb(light.color, infos[3]))
		return (1);
	if (ft_darray_append(&import->minirt->lights, &light))
		return (1);
	return (0);
}

int	process_sphere(t_import *import, char **infos)
{
	t_object	sphere;
	float		diameter;

	if (set_xyz(sphere.center, infos[1]))
		return (1);
	if (set_unit(&diameter, infos[2]))
		return (1);
	if (set_rgb(sphere.color, infos[3]))
		return (1);
	if (create_sphere(&sphere, diameter / 2.0f))
		return (1);
	if (ft_darray_append(&import->minirt->objects, &sphere))
		return (1);
	return (0);
}

int	process_plane(t_import *import, char **infos)
{
	t_object	plane;
	float		normal[4];

	if (set_xyz(plane.center, infos[1]))
		return (1);
	if (set_normal(normal, infos[2]))
		return (1);
	if (set_rgb(plane.color, infos[3]))
		return (1);
	if (create_plane(&plane, normal))
		return (1);
	if (ft_darray_append(&import->minirt->objects, &plane))
		return (1);
	return (0);
}
