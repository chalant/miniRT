/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_mapping_load.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:08:59 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:50:50 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_bmap(t_perturbator *perturbator, int fd)
{
	char	*line;
	char	**res;
	int		height;
	int		width;

	line = get_next_line(fd);
	res = ft_split(line, ' ');
	if (!res)
		return (-1);
	height = ft_atoi(res[0]);
	width = ft_atoi(res[1]);
	ft_clear_ds(res);
	if (homogeneous_matrix(&perturbator->map, height - 1, width - 1) < 0)
	{
		free(line);
		return (delete_matrix(&perturbator->map) - 1);
	}
	perturbator->perturb_normal = compute_bump;
	free(line);
	return (0);
}

int	load_bmap(t_perturbator *perturbator, int fd)
{
	char	*line;
	char	**res;
	int		i;
	int		j;

	j = 0;
	line = get_next_line(fd);
	while (line)
	{
		res = ft_split(line, ' ');
		if (!res)
			return (-1);
		i = -1;
		while (++i < perturbator->map.cols)
			perturbator->map.points[j][i] = ft_atoi(res[i]);
		ft_clear_ds(res);
		free(line);
		line = get_next_line(fd);
		j++;
	}
	free(line);
	return (0);
}

int	create_bmap(t_perturbator *pert, const char *file_path)
{
	int	fd;

	pert->map.points = NULL;
	pert->map.cols = 0;
	pert->map.rows = 0;
	fd = open(file_path, O_RDONLY);
	if (init_bmap(pert, fd) < 0)
	{
		close(fd);
		return (-1);
	}
	if (load_bmap(pert, fd) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	load_bump_maps(t_minirt *minirt)
{
	t_perturbator	pert;

	pert.map.points = NULL;
	pert.map.cols = 0;
	pert.map.rows = 0;
	pert.perturb_normal = no_perturbation;
	ft_darray_append(&minirt->perturbators, &pert);
	if (create_bmap(&pert, "resources/mesh.bmap"))
		return (delete_matrix(&pert.map) - 1);
	if (ft_darray_append(&minirt->perturbators, &pert))
		return (delete_matrix(&pert.map) - 1);
	if (create_bmap(&pert, "resources/gravel.bmap"))
		return (delete_matrix(&pert.map) - 1);
	if (ft_darray_append(&minirt->perturbators, &pert))
		return (delete_matrix(&pert.map) - 1);
	return (0);
}
