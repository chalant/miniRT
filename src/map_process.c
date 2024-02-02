/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:32:43 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/02 15:05:07 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	expected_values(int type)
{
	if (type == LIGHT || type == DIMENSION)
		return (1);
	else
		return (3);
}

int	check_integer(char *value, int type)
{
	int	i;
	int	integer;

	if (value[0] != '-' && !ft_isdigit(value[0]))
		return (1);
	i = 1;
	while (value[i])
		if (!ft_isdigit(value[i++]))
			return (1);
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
	int		bad;

	decimal = ft_atof(value, &bad);
	if (bad)
		return (1);
	else if (type == LIGHT && decimal >= 0.0 && decimal <= 1.0)
		return (0);
	else if (type == NORMAL && decimal >= -1.0 && decimal <= 1.0)
		return (0);
	else if (type == XYZ || type == DIMENSION)
		return (0);
	else
		return (1);
}

char	**check_ranges(char *values, int type)
{
	char	**tab;
	int		integer;
	float	decimal;
	int		i;

	tab = ft_split(values, ',');
	if (!tab)
		return (NULL);
	if (ft_count_strings(values) != expected_values(type))
		return (ft_clear_ds(tab), NULL);
	i = 0;
	while (tab[i])
	{
		if (type == RGB || type == FOV)
			if (check_integer(tab[i], type))
				return (ft_clear_ds(tab), NULL);
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
	char		**rgb;
	int			i;

	rgb = check_ranges(infos[2], RGB);
	if (!rgb)
		return (1);
	ambient.color[0] = ft_atoi(rgb[0]);
	ambient.color[1] = ft_atoi(rgb[1]);
	ambient.color[2] = ft_atoi(rgb[2]);
	ambient.color[3] = 0;
	import->minirt->ambient = ambient;
	ft_clear_ds(rgb);
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
	// if (ft_strcmp(infos[0], "C"))
	// 	process_camera(import, infos);
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