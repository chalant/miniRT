#include "minirt.h"

int	init_bmap(t_perturbator *perturbator, int fd)
{
	char	*line;
	char	**res;
	int		height;
	int		width;

	line  = get_next_line(fd);
	res = ft_split(line, ' ');
	height = ft_atoi(res[0]);
	width = ft_atoi(res[1]);
	ft_clear_ds(res);
	//todo: protect mallocs.
	create_matrix(&perturbator->map, height, width);
	init_matrix(&perturbator->map, 0.0f);
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

	i = -1;
	j = 0;
	line  = get_next_line(fd);
	while (line)
	{
		//todo: malloc protection
		res = ft_split(line, ' ');
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

int	load_bmaps(t_minirt *minirt)
{
	t_perturbator	pert;
	int				fd;

	pert.map.points = NULL;
	pert.perturb_normal = no_perturbation;
	ft_darray_append(&minirt->perturbators, &pert);
	//todo: protect fd
	//todo: malloc protections.
	fd = open("resources/mesh.bmap", O_RDONLY);
	init_bmap(&pert, fd);
	load_bmap(&pert, fd);
	if (ft_darray_append(&minirt->perturbators, &pert))
		return (-1);
	pert.map.points = NULL;
	fd = open("resources/gravel.bmap", O_RDONLY);
	init_bmap(&pert, fd);
	load_bmap(&pert, fd);
	if (ft_darray_append(&minirt->perturbators, &pert))
		return (-1);
	return (0);
}
