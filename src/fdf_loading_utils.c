/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_loading_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/30 14:35:23 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	delete_strings(char **numbers)
{
	int	i;

	i = 0;
	while (numbers[i])
	{
		free(numbers[i]);
		i++;
	}
	free(numbers);
}

void	set_extreme_elevation(float elevation, t_object *object, int idx)
{
	if (elevation > object->max_elevation)
	{
		object->max_elevation = elevation;
		object->max_elevation_idx = idx;
	}
	else if (elevation < object->min_elevation)
		object->min_elevation = elevation;
}

int	fill_points(t_object *object, char *line, int x, int *z)
{
	char		**numbers;
	int			y;
	t_matrix	*map;

	numbers = ft_split(line, ' ');
	if (!numbers)
		return (0);
	map = object->body;
	y = 0;
	while (*z < map->cols && numbers[y])
	{
		map->points[0][*z] = (float)x;
		map->points[1][*z] = (float)y;
		map->points[2][*z] = (float)ft_atoi(numbers[y]);
		set_extreme_elevation(map->points[2][*z], object, *z);
		object->colors[*z] = fdf_color(numbers[y]);
		object->elevations[*z] = map->points[2][*z];
		*z += 1;
		y++;
	}
	delete_strings(numbers);
	return (1);
}

int	fill_map(char *file_path, t_object *object)
{
	char	*line;
	int		fd;
	int		x;
	int		z;

	fd = open(file_path, O_RDONLY);
	line = get_next_line(fd);
	if (!line)
		return (0);
	x = 0;
	z = 0;
	object->max_elevation = 1;
	object->min_elevation = 0;
	while (line)
	{
		fill_points(object, line, x, &z);
		x++;
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	object->height = x;
	close(fd);
	return (1);
}
