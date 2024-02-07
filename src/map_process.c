/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:32:43 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/07 17:09:04 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	expected_values(int type)
{
	if (type == LIGHT || type == UNIT || type == FOV)
		return (1);
	else
		return (3);
}

int	check_integer(char *value, int type)
{
	int	integer;
	// int	i;

	// printf("From check_integer: %s\n", value);
	// if (value[0] != '-' && !ft_isdigit(value[0]))
	// 	return (1);
	// i = 1;
	// while (value[i + 1])
	// 	if (!ft_isdigit(value[i++]))
	// 		return (1);
	// if (!ft_isdigit(value[i]) && value[i] != '\n')
	// 	return (1);
	integer = ft_atoi(value);
	if (type == RGB && integer >= 0 && integer <= 255)
		return (0);
	else if (type == FOV && integer >= 0 && integer <= 180)
		return (0);
	else
		return (1);
}

int	check_decimal(char *value, int type)
{
	float	decimal;

	if (ft_atof(value, &decimal))
		return (1);
	else if (type == LIGHT && decimal >= 0.0 && decimal <= 1.0)
		return (0);
	else if (type == NORMAL && decimal >= -1.0 && decimal <= 1.0)
		return (0);
	else if (type == XYZ || type == UNIT)
		return (0);
	else
		return (1);
}

char	**check_ranges(char *values, int type)
{
	char	**tab;
	int		i;

	tab = ft_split(values, ',');
	if (!tab)
		return (NULL);
	if ((int)ft_count_strings(tab) != expected_values(type))
		return (ft_clear_ds(tab), NULL);
	i = 0;
	while (tab[i])
	{
		// printf("From check_ranges: %s\n", tab[i]);
		if (type == RGB || type == FOV)
		{
			if (check_integer(tab[i], type))
				return (ft_clear_ds(tab), printf("A\n"), NULL);
		}
		else
			if (check_decimal(tab[i], type))
				return (ft_clear_ds(tab), printf("B\n"), NULL);
		i++;
	}
	// printf("Hey from check_ranges!\n");
	return (tab);
}

int	process_ambient(t_import *import, char **infos)
{
	t_light		ambient;

	// printf("\nAmbient:\n");
	if (set_light(&ambient.brightness, infos[1]))
		return (1);
	// printf("Brightness: %.1f\n", ambient.brightness);
	if (set_rgb(ambient.color, infos[2]))
		return (1);
	// int i = -1;
	// while (++i < 4)
	// 	printf("RGB[%i]: %.1f\n", i, ambient.color[i]);
	import->minirt->ambient = ambient;
	return (0);
}

int	process_camera(t_import *import, char **infos)
{
	t_camera	camera;

	// printf("\nCamera:\n");
	if (set_xyz(camera.origin, infos[1]))
		return (1);
	// int i = -1;
	// while (++i < 4)
	// 	printf("XYZ[%i]: %.1f\n", i, camera.origin[i]);
	if (set_normal(camera.orientation, infos[2]))
		return (1);
	// i = -1;
	// while (++i < 4)
	// 	printf("NORMAL[%i]: %.1f\n", i, camera.orientation[i]);
	if (set_fov(&camera.fov, infos[3]))
		return (1);
	// printf("FOV: %.1f\n", camera.fov);
	import->minirt->camera = camera;
	return (0);
}

int	process_light(t_import *import, char **infos)
{
	t_light	light;

	// printf("\nLight:\n");
	if (set_xyz(light.position, infos[1]))
		return (1);
	// int i = -1;
	// while (++i < 4)
	// 	printf("XYZ[%i]: %.1f\n", i, light.position[i]);
	if (set_light(&light.brightness, infos[2]))
		return (1);
	// printf("Brightness: %.1f\n", light.brightness);
	if (set_rgb(light.color, infos[3]))
		return (1);	
	// i = -1;
	// while (++i < 4)
	// 	printf("RGB[%i]: %.1f\n", i, light.color[i]);
	if (ft_darray_append(&import->minirt->objects, &light))
		return (1);
	return (0);
}

int	process_sphere(t_import *import, char **infos)
{
	t_object	sphere;
	float		diameter;

	// printf("\nSphere:\n");
	if (set_xyz(sphere.center, infos[1]))
		return (1);
	// int i = -1;
	// while (++i < 4)
	// 	printf("XYZ[%i]: %.1f\n", i, sphere.center[i]);
	if (set_unit(&diameter, infos[2]))
		return (1);
	// printf("Diameter: %.1f\n", diameter);
	if (create_sphere(&sphere, diameter / 2.0f))
		return (1);
	if (set_rgb(sphere.color, infos[3]))
		return (1);
	// i = -1;
	// while (++i < 4)
	// 	printf("RGB[%i]: %.1f\n", i, sphere.color[i]);
	if (ft_darray_append(&import->minirt->objects, &sphere))
		return (1);
	return (0);
}

int	process_plane(t_import *import, char **infos)
{
	t_object	plane;

	// printf("\nPlane:\n");
	if (set_xyz(plane.center, infos[1]))
		return (1);
	// int i = -1;
	// while (++i < 4)
	// 	printf("XYZ[%i]: %.1f\n", i, plane.center[i]);
	if (set_normal(plane.orientation, infos[2]))
		return (1);
	// i = -1;
	// while (++i < 4)
	// 	printf("NORMAL[%i]: %.1f\n", i, plane.orientation[i]);
	if (set_rgb(plane.color, infos[3]))
		return (1);
	// i = -1;
	// while (++i < 4)
	// 	printf("RGB[%i]: %.1f\n", i, plane.color[i]);
	if (ft_darray_append(&import->minirt->objects, &plane))
		return (1);
	return (0);
}

// int	process_cylinder(t_import *import, char **infos)
// {
// 	t_object	cylinder;
// }

// int	process_cone(t_import *import, char **infos)	TODO: T_CONE STRUCT
// {
// 	t_object	cone;
// }

// [ ] TODO: Manage array deletion if an error occur
int	process_element(t_import *import, char **infos)
{
	// printf("Hey from process element\n");
	if (!ft_strcmp(infos[0], "A"))
		return (process_ambient(import, infos));
	if (!ft_strcmp(infos[0], "C"))
		return (process_camera(import, infos));
	if (!ft_strcmp(infos[0], "L"))
		return (process_light(import, infos));
	if (!ft_strcmp(infos[0], "sp"))
		return (process_sphere(import, infos));
	if (!ft_strcmp(infos[0], "pl"))
		return (process_plane(import, infos));
	// if (!ft_strcmp(infos[0], "cy"))
	// 	return (process_cylinder(import, infos));
// 	// if (!ft_strcmp(infos[0], "cn"))
// 	// 	return (process_cone(import, infos));
	else
		return (1); // ft_darray_delete if something already exits, othwewise just ret 1
}
