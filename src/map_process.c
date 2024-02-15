/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:32:43 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/15 20:40:14 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	expected_values(int type)
{
	if (type == LIGHT || type == UNIT || type == FOV || type == INDEX)
		return (1);
	else
		return (3);
}

int	check_integer(char *value, int type)
{
	int	integer;
	int	sus;
	int	i;

	if (value[0] != '-' && !ft_isdigit(value[0]))
		return (1);
	sus = 0;
	i = 0;
	while (value[++i])
		if (!ft_isdigit(value[i]))
			sus++;
	if (sus > 1 || (sus == 1 && value[ft_strlen(value) - 1] != '\n'))
		return (1);
	integer = ft_atoi(value);
	if (type == RGB && integer >= 0 && integer <= 255)
		return (0);
	else if (type == FOV && integer >= 0 && integer <= 180)
		return (0);
	else if (type == INDEX && integer >= 0)
		return (0);
	else
		return (1);
}

int	check_decimal(char *value, int type)
{
	float	decimal;

	if (ft_atof(value, &decimal))
		return (1);
	if (type == LIGHT && decimal >= 0.0 && decimal <= 1.0)
		return (0);
	else if (type == NORMAL)
		return (0);
	else if (type == UNIT && decimal > 0.0)
		return (0);
	else if (type == XYZ)
		return (0);
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
		if (type == RGB || type == FOV || type == INDEX)
		{
			if (check_integer(tab[i], type))
				return (ft_clear_ds(tab), NULL);
		}
		else
			if (check_decimal(tab[i], type))
				return (ft_clear_ds(tab), NULL);
		i++;
	}
	return (tab);
}

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
	if (ft_darray_append(&import->minirt->spot_lights, &light))
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
	if (import->material)
		if (set_index(&sphere.material_index, infos[4]))
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
	if (import->material)
		if (set_index(&plane.material_index, infos[4]))
			return (1);
	if (create_plane(&plane, normal))
		return (1);
	if (ft_darray_append(&import->minirt->objects, &plane))
		return (1);
	return (0);
}

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
	if (import->material)
		if (set_index(&cylinder.material_index, infos[6]))
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
	if (import->material)
		if (set_index(&cone.material_index, infos[6]))
			return (1);
	if (create_cone(&cone, height, diameter))
		return (1);
	if (ft_darray_append(&import->minirt->objects, &cone))
		return (1);
	return (0);
}

int	process_material(t_import *import, char **infos)
{
	t_material	material;

	if (ft_atof(&material.diffuse_reflection, infos[1]))
		return (1);
	if (ft_atof(&material.ambient_reflection, infos[2]))
		return (1);
	if (ft_atof(&material.specular_reflection, infos[3]))
		return (1);
	if (ft_atof(&material.shininess, infos[4]))
		return (1);
	if (ft_atof(&material.reflectivity, infos[5]))
		return (1);
	if (set_rgb(material.color, infos[6]))
		return (1);
	if (ft_atof(&material.repeat_pattern, infos[7]))
		return (1);
	if (ft_darray_append(&import->minirt->materials, &material))
		return (1);
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
	else if (!ft_strcmp(infos[0], "mt"))
		return (process_material(import, infos));
	else
		return (1);
}
