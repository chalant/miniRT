/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:30:29 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/06 16:31:29 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include "minirt.h"
# include "get_next_line.h"
# include <fcntl.h>

typedef struct s_import
{
	t_minirt	*minirt;
	int			fd;
	int			ambient;
	int			camera;
	int			light;
}	t_import;

typedef enum e_ranges
{
	LIGHT,
	XYZ,
	RGB,
	NORMAL,
	DIMENSION,
	FOV,
}	t_ranges;

// Map
int		check_extension(char *filename, char *extension);
int		init_import(t_import *import, t_minirt *minirt, char *map);
int		check_scene(t_import *import);
int		import_map(t_minirt *minirt, char **av);

// Read
int		is_blank(char **line);
int		expected_infos(char *type);
int		seen_type(t_import *import, char **line);
int		read_map(t_import *import);

// Process
int		expected_values(int type);
int		check_integer(char *value, int type);
int		check_decimal(char *value, int type);
char	**check_ranges(char *values, int type);
int		process_element(t_import *import, char **infos);

// Processes
int		process_ambient(t_import *import, char **infos);
// int	process_camera(t_import *import, char **infos);
// int	process_light(t_import *import, char **infos);
// int	process_sphere(t_import *import, char **infos);
// int	process_plane(t_import *import, char **infos);
// int	process_cylinder(t_import *import, char **infos);
// int	process_cone(t_import *import, char **infos)

// Process Utils
int		set_light(float light, char *info);
int		set_rgb(int *rgb, char *info);
int		set_xyz(float *xyz, char *info);
int		set_normal(float *normal, char *info);
int		set_fov(int	fov, char *info);

// Argument to float
int		check_input(char *nbr);
float	check_sign(char *nbr);
int		ft_atof(char *nbr, float *res);

#endif
