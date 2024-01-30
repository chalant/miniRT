/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                            :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/21 13:15:28 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "minirt_objects.h"
#include <stdio.h>

int	set_minirt_transforms(t_minirt *minirt)
{
	if (set_rotations(minirt, 2.1f, 2.1f, 2.1f) < 0)
		return (0);
	if (set_translations(minirt, 0.1f, 0.1f, 0.1f) < 0)
		return (0);
	// if (!set_scalings(minirt, 1.1f, 1.1f, 1.1f))
	// 	return (0);
	// if (!set_zooming(minirt, 1.1f))
	// 	return (0);
	// homogeneous_matrix(&minirt->transforms, 3, 3);
	// identity_matrix(minirt->transforms, 3, 3);
	// homogeneous_matrix(&minirt->centering, 3, 3);
	// translation(minirt->centering, minirt->display->height / 2.0f,
	// 	minirt->display->width / 2.0f, 0.0f);
	// homogeneous_matrix(&minirt->tmp, 3, 3);
	return (1);
}

int	mlx_setup(t_minirt *minirt)
{
	t_display	*display;

	display = minirt->display;
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

//todo: hard code the rotations.
int	mouse_update(int x, int y, t_minirt *minirt)
{
	float	cos_a;
	float	sin_a;
	float	dx;
	float	dy;

	if (!minirt->mouse.capture)
		return (0);
	dy = (y - minirt->display->height / 2);
	dx = (x - minirt->display->width / 2);
	if (dy == 0.0f && dx == 0.0f)
		return (0);
	if (dy != 0.0f)
		dy /= fabsf(dy);
	if (dx != 0.0f)
		dx /= fabsf(dx);
	dx *= 0.02f;
	dy *= 0.02f;
	cos_a = cosf(dy);
	sin_a = sinf(dy);
	minirt->mouse.direction->points[0][0] = 1.0f;
	minirt->mouse.direction->points[1][1] = cos_a;
	minirt->mouse.direction->points[2][1] = sin_a;
	minirt->mouse.direction->points[1][2] = -sin_a;
	minirt->mouse.direction->points[2][2] = cos_a;
	matmul(minirt->camera->basis, minirt->mouse.direction, minirt->tmp, 3);
	matrix_copy(minirt->tmp, minirt->camera->basis, 3);
	if (dx == 0.0f)
		return (0);
	cos_a = cosf(dx);
	sin_a = sinf(dx);
	minirt->mouse.direction->points[0][0] = 0.0f;
	minirt->mouse.direction->points[1][1] = 0.0f;
	minirt->mouse.direction->points[2][1] = 0.0f;
	minirt->mouse.direction->points[1][2] = 0.0f;
	minirt->mouse.direction->points[2][2] = 0.0f;
	minirt->mouse.direction->points[0][0] = cos_a;
	minirt->mouse.direction->points[0][2] = -sin_a;
	minirt->mouse.direction->points[1][1] = 1.0f;
	minirt->mouse.direction->points[2][0] = sin_a;
	minirt->mouse.direction->points[2][2] = cos_a;
	matmul(minirt->camera->basis, minirt->mouse.direction, minirt->tmp, 3);
	matrix_copy(minirt->tmp, minirt->camera->basis, 3);
	look_at(minirt->camera, minirt);
	minirt->mouse.direction->points[0][0] = 0.0f;
	minirt->mouse.direction->points[0][2] = 0.0f;
	minirt->mouse.direction->points[1][1] = 0.0f;
	minirt->mouse.direction->points[2][0] = 0.0f;
	minirt->mouse.direction->points[2][2] = 0.0f;
	//mlx_mouse_move(minirt->window, minirt->display->width / 2, minirt->display->height / 2);
	return (0);
}

int	set_hooks(t_minirt *minirt)
{
	//mlx_hook(minirt->window, 6, 1L << 6, mouse_update, minirt);
	mlx_hook(minirt->window, 2, 1L << 0, key_press_hook, minirt);
	mlx_hook(minirt->window, 3, 1L << 1, key_release_hook, minirt);
	mlx_hook(minirt->window, 17, 0, close_program, minirt);
	mlx_expose_hook(minirt->window, handle_expose, minirt);
	mlx_loop_hook(minirt->mlx, update_view, minirt);
	return (1);
}

int	set_variables(t_minirt *minirt)
{
	minirt->display->height = 700;
	minirt->display->width = 800;
	return (1);
}

int	load_scene(t_minirt *minirt)
{
	t_object	new;

	minirt->camera = malloc(sizeof(t_camera));
	if (!minirt->camera)
		return (-1);
	minirt->diffuse.direction[0] = 1.0f;
	minirt->diffuse.direction[1] = -1.0f;
	minirt->diffuse.direction[2] = -1.0f;
	minirt->diffuse.direction[3] = 0.0f;
	//scale_vector(minirt->light->direction, -1.0f, 3);
	normalize_vector(minirt->diffuse.direction, minirt->diffuse.direction, 3);
	scale_vector(minirt->diffuse.direction, -1.0f, minirt->diffuse.direction, 3);
	//todo: the light direction should be the negative of the position, normalized.
	minirt->diffuse.brightness = 1.0f;
	to_color(0x00ffffff, minirt->diffuse.color);
	//todo: malloc protection
	minirt->camera->fov = 60.0f;
	minirt->camera->near = -1.0f;
	minirt->camera->far = 1.0f;
	minirt->camera->orientation = ft_calloc(4, sizeof(float));
	minirt->camera->orientation[0] = 0.0f;
	minirt->camera->orientation[1] = 0.0f;
	minirt->camera->orientation[2] = -1.0f;
	minirt->camera->orientation[3] = 1.0f;
	normalize_vector(minirt->camera->orientation, minirt->camera->orientation, 3);
	minirt->camera->origin[0] = 0.0f;
	minirt->camera->origin[1] = 0.0f;
	minirt->camera->origin[2] = 3.0f;
	minirt->camera->origin[3] = 1.0f;
	if (set_camera_transform(minirt->camera) == -1)
		return (-1);
	create_sphere(&new, 0.7f, "red");
	to_color(0x00f94449, new.color);
	new.name = "red";
	new.center = ft_calloc(4, sizeof(float));
	new.center[0] = 0.8f;
	new.center[1] = -1.2f;
	new.center[2] = 0.0f;
	new.center[3] = 1.0f;
	new.albedo = 0.5f;
	ft_darray_append(minirt->objects, &new);
	create_sphere(&new, 0.4f, "blue");
	to_color(0x003261e3, new.color);
	new.name = "blue";
	new.center = ft_calloc(4, sizeof(float));
	new.center[0] = 0.0f;
	new.center[1] = -0.3f;
	new.center[2] = 0.0f;
	new.center[3] = 1.0f;
	new.albedo = 0.9f;
	ft_darray_append(minirt->objects, &new);
	create_sphere(&new, 0.5f, "green");
	to_color(0x11aaea8c, new.color);
	new.center = ft_calloc(4, sizeof(float));
	new.center[0] = -0.5f;
	new.center[1] = -1.4f;
	new.center[2] = -0.0f;
	new.center[3] = 0.0f;
	new.albedo = 0.5f;
	ft_darray_append(minirt->objects, &new);
	create_plane(&new, (float[4]){0.0f, 1.0f, 0.0f, 0.0f});
	new.center = ft_calloc(4, sizeof(float));
	to_color(0x00ffffff, new.color);
	new.center[0] = 0.0f;
	new.center[1] = -2.0f;
	new.center[2] = 0.0f;
	new.center[3] = 1.0f;
	new.albedo = 1.0f;
	ft_darray_append(minirt->objects, &new);
	// create_plane(&new, (float[4]){1.0f, 0.0f, 0.0f, 0.0f});
	// new.center = ft_calloc(4, sizeof(float));
	// to_color(0x00ffff00, new.color);
	// new.center[0] = 2.0f;
	// new.center[1] = 0.0f;
	// new.center[2] = 0.0f;
	// new.center[3] = 1.0f;
	// ft_darray_append(minirt->objects, &new);
	// create_plane(&new, (float[4]){0.0f, 0.0f, 1.0f, 0.0f});
	// new.center = ft_calloc(4, sizeof(float));
	// to_color(0x00ffff00, new.color);
	// new.center[0] = 0.0f;
	// new.center[1] = 0.0f;
	// new.center[2] = 2.0f;
	// new.center[3] = 1.0f;
	// ft_darray_append(minirt->objects, &new);
	// create_plane(&new, (float[4]){0.0f, 1.0f, 0.0f, 0.0f});
	// new.center = ft_calloc(4, sizeof(float));
	// to_color(0x00ffffff, new.color);
	// new.center[0] = 0.0f;
	// new.center[1] = -2.0f;
	// new.center[2] = 0.0f;
	// new.center[3] = 1.0f;
	// ft_darray_append(minirt->objects, &new);
	// create_plane(&new, (float[4]){1.0f, 0.0f, 0.0f, 0.0f});
	// new.center = ft_calloc(4, sizeof(float));
	// to_color(0x00ffff00, new.color);
	// new.center[0] = -2.0f;
	// new.center[1] = 0.0f;
	// new.center[2] = 0.0f;
	// new.center[3] = 1.0f;
	// ft_darray_append(minirt->objects, &new);
	return (1);
}

int	main(int argc, char *argv[])
{
	t_minirt	minirt;
	t_display	display;

	//todo: use a cache for this.
	(void)argv;
	if (argc - 1 < 0 || argc - 1 > 1)
		return (0);
	minirt_init(&minirt);
	minirt.display = &display;
	minirt.mouse.x = 0.0f;
	minirt.mouse.y = 0.0f;
	set_variables(&minirt);
	mlx_setup(&minirt);
	set_hooks(&minirt);
	set_minirt_transforms(&minirt);
	homogeneous_matrix(&minirt.tmp, 3, 3);
	load_scene(&minirt);
	perspective_projector(&minirt.world_space, minirt.display, minirt.camera);
	invert_matrix(minirt.world_space, minirt.world_space, minirt.tmp, 4);
	perspective_projector(&minirt.view_matrix, minirt.display, minirt.camera);
	mlx_loop(minirt.mlx);
	return (0);
}
