/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:32:43 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/01 15:37:22 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

// WIP
int	expected_values(int type)
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

char	**check_ranges(char *values, int type)
{
	char	**tab;
	int		i;

	tab = ft_split(values, ',');
	if (!tab || ft_lstsize(values) != type)
		return (1);
	while (tab[i])
	{
		if (type == LIGHT && ft_atof(tab[i]) >= 0.0 && ft_atof(tab[i]) <= 1.0)
			return (0);
		if (type == RGB && ft_atoi(tab[i]) >= 0 && ft_atoi(tab[i]) <= 255)
			return (0);
		if (type == NORMAL && ft_atof(tab[i]) >= -1.0 && ft_atof(tab[i]) <= 1.0)
			return (0);
		if (type == FOV && ft_atoi(tab[i]) >= 0 && (int)tab[i] <= 180)
			return (0);
		i++;
	}
	return (1);
}

// Plug-in the check_ranges() function
int	process_ambient(t_import *import, char **infos)
{
	t_light		ambient;
	char		**colors;
	int			i;

	colors = check_ranges(infos[2], RGB);
	if (!colors)
		return (1);
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