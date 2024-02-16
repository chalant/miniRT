/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/12 15:31:32 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "map.h"
#include "minirt_controls.h"
#include "minirt_objects.h"
#include <stdio.h>

int	set_minirt_transforms(t_minirt *minirt)
{
	if (set_rotations(minirt, 2.1f, 2.1f, 2.1f) < 0)
		return (0);
	return (1);
}

int	mlx_setup(t_minirt *minirt)
{
	t_display	*display;

	display = &minirt->display;
	display->aspect_ratio = (float)display->width / (float)display->height;
	minirt->mlx = mlx_init();
	if (!minirt->mlx)
		return (-1);
	minirt->window = mlx_new_window(minirt->mlx,
		display->width, display->height, "miniRT");
	if (!minirt->window)
		return (-1);
	display->img = mlx_new_image(minirt->mlx,
		display->width, display->height);
	if (!display->img)
		return (-1);
	display->addr = mlx_get_data_addr(display->img, &display->bits_per_pixel,
			&display->line_length, &display->endian);
	display->offset = display->bits_per_pixel / 8;
	if (!display->addr)
		return (0);
	return (1);
}

int	print_matrix(t_matrix *matrix)
{
	int	i;
	int	j;

	i = -1;
	while (++i < matrix->rows)
	{
		j = -1;
		while (++j < matrix->cols)
			fprintf(stderr, "%f ", matrix->points[i][j]);
		fprintf(stderr, "\n");
	}
	return (0);
}

int	normalize_col(t_matrix *matrix, int col)
{
	int		i;
	int		mag;

	i = -1;
	mag = 0;
	while (++i < matrix->rows)
		mag += matrix->points[i][col] * matrix->points[i][col];
	mag = sqrt(mag);
	i = -1;
	while (++i < matrix->rows)
		matrix->points[i][col] /= mag;
	return (0);
}

int	set_hooks(t_minirt *minirt)
{
	mlx_hook(minirt->window, 6, 1L << 6, mouse_update, minirt);
	mlx_hook(minirt->window, 4, 1L << 2, mouse_click_hook, minirt);
	mlx_hook(minirt->window, 5, 1L << 3, mouse_release_hook, minirt);
	mlx_hook(minirt->window, 2, 1L << 0, key_press_hook, minirt);
	mlx_hook(minirt->window, 3, 1L << 1, key_release_hook, minirt);
	mlx_hook(minirt->window, 17, 0, close_program, minirt);
	mlx_expose_hook(minirt->window, handle_expose, minirt);
	mlx_loop_hook(minirt->mlx, update_view, minirt);
	return (1);
}

int	set_variables(t_minirt *minirt)
{
	minirt->display.height = 600;
	minirt->display.width = 800;
	return (1);
}

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

int	set_materials(t_minirt *minirt)
{
	t_material	material;
	t_material	other;
	t_texture	txt;
	t_object	*object;
	t_perturbator	pert;

	//todo: need to load the bump map file...
	//the file could be specified in the rt file, and would load the
	//bump map file when parsing.
	material.ambient_reflection = 0.2f;
	material.diffuse_reflection = 0.8f;
	material.specular_reflection = 0.8f;
	material.shininess = 200.0f;
	material.reflectivity = 0.05f;
	material.repeat_pattern = 10.0f;

	txt.get_texture = no_texture;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = checkerboard;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = horizontal_bands;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = vertical_bands;
	ft_darray_append(&minirt->textures, &txt);

	other.ambient_reflection = 0.2f;
	other.diffuse_reflection = 0.8f;
	other.specular_reflection = 0.2f;
	other.shininess = 20.5f;
	other.reflectivity = 0.2f;
	other.repeat_pattern = 0.01f;

	pert.perturb_normal = no_perturbation;
	ft_darray_append(&minirt->perturbators, &pert);
	load_bmap(minirt, "resources/mesh.bmap");
	to_color(0x00ffffff, material.color);
	to_color(0x00ffffff, other.color);
	load_bmap(minirt, "resources/gravel.bmap");
	ft_darray_append(&minirt->materials, &material);
	ft_darray_append(&minirt->materials, &other);
	
	object = ft_darray_get(&minirt->objects, 0);
	object->material_index = 1;
	object->perturbator_index = 1;
	object->texture_index = 0;
	
	object = ft_darray_get(&minirt->objects, 1);
	object->material_index = 0;
	object->perturbator_index = 2;
	object->texture_index = 1;
	
	object = ft_darray_get(&minirt->objects, 2);
	object->material_index = 0;
	object->perturbator_index = 0;
	object->texture_index = 2;
	
	object = ft_darray_get(&minirt->objects, 3);
	object->material_index = 0;
	object->perturbator_index = 0;
	object->texture_index = 3;
	return (0);
}

int	main(int argc, char *argv[])
{
	t_minirt	minirt;

	if (argc != 2)
		return (1);
	minirt_init(&minirt);
	//todo: call free here 
	if (import_map(&minirt, argv))
		return (1);
	homogeneous_matrix(&minirt.tmp, 3, 3);
	minirt.mouse.x = 0.0f;
	minirt.mouse.y = 0.0f;
	set_variables(&minirt);
	mlx_setup(&minirt);
	set_hooks(&minirt);
	set_minirt_transforms(&minirt);
	to_color(0x0087ceeb, minirt.sky_color);
	normalize_vector(minirt.camera.orientation, minirt.camera.orientation, 3);
	set_materials(&minirt);
	minirt.camera.near = -1.0f;
	minirt.camera.far = 1.0f;
	if (set_camera_transform(&minirt, &minirt.camera) == -1)
		return (-1);
	perspective_projector(&minirt.world_space, &minirt.display, &minirt.camera);
	invert_matrix(&minirt.world_space, &minirt.world_space, &minirt.tmp, 4);
	minirt.render_mode = full_resolution;
	mlx_loop(minirt.mlx);
	return (0);
}
