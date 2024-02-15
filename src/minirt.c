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
	// if (set_translations(minirt, 0.1f, 0.1f, 0.1f) < 0)
	// 	return (0);
	return (1);
}

int	mlx_setup(t_minirt *minirt)
{
	t_display	*display;

	display = &minirt->display;
	//todo: malloc protection
	display->origin = ft_calloc(4, sizeof(float));
	display->origin[0] = 0.0f;
	display->origin[1] = 0.0f;
	display->origin[3] = 0.0f;
	display->aspect_ratio = (float)display->width / (float)display->height;
	minirt->mlx = mlx_init();
	if (!minirt->mlx)
		return (0);
	minirt->window = mlx_new_window(minirt->mlx,
		display->width, display->height, "miniRT");
	if (!minirt->window)
		return (0);
	display->img = mlx_new_image(minirt->mlx,
		display->width, display->height);
	if (!display->img)
		return (0);
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

int	load_bmap(t_material *material, char *file_path)
{
	int		fd;
	char	*line;
	char	**res;
	int		height;
	int		width;
	int		i;

	fd = open(file_path, O_RDONLY);
	line  = get_next_line(fd);
	res = ft_split(line, ' ');
	free(line);
	height = ft_atoi(res[0]);
	width = ft_atoi(res[1]);
	ft_clear_ds(res);
	create_matrix(&material->bump_map, height, width);
	init_matrix(&material->bump_map, 0.0f);
	i = -1;
	int	j = 0;
	line  = get_next_line(fd);
	while (line)
	{
		res = ft_split(line, ' ');
		i = -1;
		while (++i < width)
			material->bump_map.points[j][i] = ft_atoi(res[i]);
		ft_clear_ds(res);
		free(line);
		line = get_next_line(fd);
		j++;
	}
	free(line);
	return (0);
}

int	load_scene(t_minirt *minirt)
{
	//todo: need to set object basis for transformations.
	t_object	new;
	t_material	material;
	t_material	other;

	//todo: need to load the bump map file...
	//the file could be specified in the rt file, and would load the
	//bump map file when parsing.
	material.ambient_reflection = 0.2f;
	material.diffuse_reflection = 0.8f;
	material.shininess = 100.0f;
	material.reflectivity = 0.2f;
	material.repeat_pattern = 6.0f;
	material.get_texture = checkerboard;
	material.normal_perturb = compute_bump;

	other.ambient_reflection = 0.8f;
	other.diffuse_reflection = 0.8f;
	other.shininess = 100.5f;
	other.reflectivity = 0.2f;
	other.repeat_pattern = 0.2f;
	other.get_texture = checkerboard;
	other.normal_perturb = compute_bump;

	load_bmap(&material, "resources/mesh.bmap");
	//to_color(0x00f94449, material.dark_color);
	to_color(0x00ffffff, material.color);
	to_color(0x00ffff00, other.color);
	load_bmap(&other, "resources/gravel.bmap");
	ft_darray_append(&minirt->materials, &material);
	ft_darray_append(&minirt->materials, &other);
	minirt->diffuse.position[0] = -40.0f;
	minirt->diffuse.position[1] = 30.0f;
	minirt->diffuse.position[2] = 0.0f;
	minirt->diffuse.brightness = 0.8f;
	to_color(0x00ffffff, minirt->diffuse.color);
	minirt->ambient.brightness = 0.2f;
	to_color(0x00ffffff, minirt->ambient.color);
	//todo: malloc protection
	minirt->camera.fov = 70.0f;
	minirt->camera.near = -1.0f;
	minirt->camera.far = 1.0f;
	minirt->camera.orientation[0] = 0.0f;
	minirt->camera.orientation[1] = 0.0f;
	minirt->camera.orientation[2] = -1.0f;
	minirt->camera.orientation[3] = 1.0f;
	normalize_vector(minirt->camera.orientation, minirt->camera.orientation, 3);
	minirt->camera.origin[0] = 0.0f;
	minirt->camera.origin[1] = 0.0f;
	minirt->camera.origin[2] = 3.0f;
	minirt->camera.origin[3] = 1.0f;
	if (set_camera_transform(minirt, &minirt->camera) == -1)
		return (-1);
	create_cone(&new, 2.0f, 1.0f);
	to_color(0x00f94449, new.color);
	new.name = "blue sphere";
	new.center[0] = 0.0f;
	new.center[1] = 0.0f;
	new.center[2] = 0.0f;
	new.center[3] = 1.0f;
	new.material = ft_darray_get(&minirt->materials, 0);
	ft_darray_append(&minirt->objects, &new);
	create_sphere(&new, 0.5f);
	to_color(0x11aaea8c, new.color);
	new.name = "green sphere";
	new.center[0] = -0.9f;
	new.center[1] = -1.4f;
	new.center[2] = -0.0f;
	new.center[3] = 0.0f;
	new.material = ft_darray_get(&minirt->materials, 0);
	ft_darray_append(&minirt->objects, &new);
	create_plane(&new, (float[4]){0.0f, 1.0f, 0.0f, 0.0f});
	new.name = "plane";
	to_color(0x003261e3, new.color);
	new.center[0] = 0.0f;
	new.center[1] = -2.0f;
	new.center[2] = 0.0f;
	new.center[3] = 1.0f;
	new.material = ft_darray_get(&minirt->materials, 1);
	ft_darray_append(&minirt->objects, &new);
	create_sphere(&new, 1.5f);
	to_color(0x00f94449, new.color);
	new.name = "red sphere";
	new.center[0] = 0.8f;
	new.center[1] = -0.2f;
	new.center[2] = 0.0f;
	new.center[3] = 0.0f;
	new.material = ft_darray_get(&minirt->materials, 0);
	ft_darray_append(&minirt->objects, &new);
	return (1);
}

// int	main(int argc, char *argv[])
// {
// 	t_minirt	minirt;

// 	//todo: use a cache for this.
// 	(void)argv;
// 	if (argc - 1 < 0 || argc - 1 > 1)
// 		return (0);
// 	minirt_init(&minirt);
// 	minirt.mouse.x = 0.0f;
// 	minirt.mouse.y = 0.0f;
// 	set_variables(&minirt);
// 	mlx_setup(&minirt);
// 	set_hooks(&minirt);
// 	set_minirt_transforms(&minirt);
// 	homogeneous_matrix(&minirt.tmp, 3, 3);
// 	load_scene(&minirt);
// 	perspective_projector(&minirt.world_space, &minirt.display, &minirt.camera);
// 	invert_matrix(&minirt.world_space, &minirt.world_space, &minirt.tmp, 4);
// 	perspective_projector(&minirt.view_matrix, &minirt.display, &minirt.camera);
// 	mlx_loop(minirt.mlx);
// 	return (0);
// }

int	load_materials(t_minirt *minirt)
{
	t_material	material;
	t_material	other;
	t_object	*object;
	// t_light		*diffuse;

	//todo: need to load the bump map file...
	//the file could be specified in the rt file, and would load the
	//bump map file when parsing.
	material.ambient_reflection = 0.2f;
	material.diffuse_reflection = 0.8f;
	material.specular_reflection = 0.8f;
	material.shininess = 20.0f;
	material.reflectivity = 0.05f;
	material.repeat_pattern = 10.0f;
	material.get_texture = checkerboard;
	material.normal_perturb = compute_bump;

	other.ambient_reflection = 0.2f;
	other.diffuse_reflection = 0.8f;
	other.specular_reflection = 0.2f;
	other.shininess = 20.5f;
	other.reflectivity = 0.0f;
	other.repeat_pattern = 0.1f;
	other.get_texture = checkerboard;
	other.normal_perturb = compute_bump;

	load_bmap(&material, "resources/mesh.bmap");
	//to_color(0x00f94449, material.dark_color);
	to_color(0x00ffffff, material.color);
	to_color(0x00ffffff, other.color);
	load_bmap(&other, "resources/gravel.bmap");
	ft_darray_append(&minirt->materials, &material);
	ft_darray_append(&minirt->materials, &other);
	object = ft_darray_get(&minirt->objects, 0);
	object->material = ft_darray_get(&minirt->materials, 1);
	object = ft_darray_get(&minirt->objects, 1);
	object->material = ft_darray_get(&minirt->materials, 0);
	object = ft_darray_get(&minirt->objects, 2);
	object->material = ft_darray_get(&minirt->materials, 0);
	object = ft_darray_get(&minirt->objects, 3);
	object->material = ft_darray_get(&minirt->materials, 0);

	// to_color(0x00ffffff, minirt->diffuse.color);
	// minirt->diffuse.position[0] = -40.0f;
	// minirt->diffuse.position[1] = -30.0f;
	// minirt->diffuse.position[2] = 0.7f;
	// minirt->diffuse.brightness = 0.7f;
	// ft_darray_append(&minirt->spot_lights, &minirt->diffuse);
	t_light	light;
	to_color(0x00ff00ff, light.color);
	light.position[0] = -40.0f;
	light.position[1] = 30.0f;
	light.position[2] = 0.7f;
	light.brightness = 0.9f;
	ft_darray_append(&minirt->spot_lights, &light);
	//diffuse = ft_darray_get(&minirt->spot_lights, 0);
	// diffuse->position[0] = -40.0f;
	// diffuse->position[1] = 30.0f;
	// diffuse->position[2] = 0.0f;
	// diffuse->brightness = 0.8f;
	// to_color(0x00ffffff, diffuse->color);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_minirt	minirt;

	if (argc != 2)
		return (1);
	minirt_init(&minirt);
	minirt.mouse.x = 0.0f;
	minirt.mouse.y = 0.0f;
	set_variables(&minirt);
	mlx_setup(&minirt);
	set_hooks(&minirt);
	set_minirt_transforms(&minirt);
	homogeneous_matrix(&minirt.tmp, 3, 3);
	import_map(&minirt, argv);
	to_color(0x0087ceeb, minirt.sky_color);
	normalize_vector(minirt.camera.orientation, minirt.camera.orientation, 3);
	load_materials(&minirt);
	minirt.camera.near = -1.0f;
	minirt.camera.far = 1.0f;
	if (set_camera_transform(&minirt, &minirt.camera) == -1)
		return (-1);
	perspective_projector(&minirt.world_space, &minirt.display, &minirt.camera);
	invert_matrix(&minirt.world_space, &minirt.world_space, &minirt.tmp, 4);
	perspective_projector(&minirt.view_matrix, &minirt.display, &minirt.camera);
	mlx_loop(minirt.mlx);
	return (0);
}
