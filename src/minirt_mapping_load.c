#include "minirt.h"

int	load_bmap(t_minirt *minirt, char *file_path)
{
	int				fd;
	char			*line;
	char			**res;
	int				height;
	int				width;
	int				i;
	t_perturbator	bump_mapping;

	fd = open(file_path, O_RDONLY);
	line  = get_next_line(fd);
	res = ft_split(line, ' ');
	free(line);
	height = ft_atoi(res[0]);
	width = ft_atoi(res[1]);
	ft_clear_ds(res);
	bump_mapping.map.points = NULL;
	create_matrix(&bump_mapping.map, height, width);
	init_matrix(&bump_mapping.map, 0.0f);
	bump_mapping.perturb_normal = compute_bump;
	i = -1;
	int	j = 0;
	line  = get_next_line(fd);
	while (line)
	{
		res = ft_split(line, ' ');
		i = -1;
		while (++i < width)
			bump_mapping.map.points[j][i] = ft_atoi(res[i]);
		ft_clear_ds(res);
		free(line);
		line = get_next_line(fd);
		j++;
	}
	if (ft_darray_append(&minirt->perturbators, &bump_mapping))
		return (-1);
	free(line);
	return (0);
}

int	load_bmaps(t_minirt *minirt)
{
	t_perturbator	pert;

	pert.map.points = NULL;
	pert.perturb_normal = no_perturbation;
	ft_darray_append(&minirt->perturbators, &pert);
	load_bmap(minirt, "resources/mesh.bmap");
	load_bmap(minirt, "resources/gravel.bmap");
	return (0);
}
