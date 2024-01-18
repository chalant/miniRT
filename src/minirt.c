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

int	set_hooks(t_minirt *minirt)
{
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
	minirt->light = malloc(sizeof(t_light));
	if (!minirt->light)
		return (-1);
	minirt->light->direction[0] = -1.0f;
	minirt->light->direction[1] = 1.0f;
	minirt->light->direction[2] = 2.0f;
	minirt->light->direction[3] = 1.0f;
	//scale_vector(minirt->light->direction, -1.0f, 3);
	normalize_vector(minirt->light->direction, 3);
	//todo: the light direction should be the negative of the position, normalized.
	minirt->light->brightness = 1.0f;
	to_color(0x00eafafe, minirt->light->color);
	//todo: malloc protection
	minirt->camera->fov = 90.0f;
	minirt->camera->near = -1.0f;
	minirt->camera->far = 1.0f;
	minirt->camera->orientation = ft_calloc(4, sizeof(float));
	minirt->camera->orientation[0] = 0.0f;
	minirt->camera->orientation[1] = 0.0f;
	minirt->camera->orientation[2] = 1.0f;
	minirt->camera->orientation[3] = 1.0f;
	minirt->camera->origin = ft_calloc(3, sizeof(float));
	minirt->camera->origin[0] = 0.0f;
	minirt->camera->origin[1] = 0.0f;
	minirt->camera->origin[2] = 0.0f;
	minirt->camera->ray_direction = ft_calloc(4, sizeof(float));
	minirt->camera->ray_direction[3] = 0.0f;
	if (set_camera_transform(minirt->camera) == -1)
		return (-1);
	create_sphere(&new, 0.2f);
	to_color(0x00f94449, new.color);
	new.center = ft_calloc(4, sizeof(float));
	new.center[0] = 0.0f;
	new.center[1] = 0.0f;
	new.center[2] = -4.0f;
	new.center[3] = 1.0f;
	ft_darray_append(minirt->objects, &new);
	create_sphere(&new, 0.2f);
	to_color(0x003261e3, new.color);
	new.center = ft_calloc(4, sizeof(float));
	new.center[0] = -0.5f;
	new.center[1] = 0.5f;
	new.center[2] = -2.0f;
	new.center[3] = 1.0f;
	ft_darray_append(minirt->objects, &new);
	create_sphere(&new, 0.5f);
	to_color(0x11aaea8c, new.color);
	new.center = ft_calloc(4, sizeof(float));
	new.center[0] = -0.5f;
	new.center[1] = 0.0f;
	new.center[2] = -1.0f;
	new.center[3] = 1.0f;
	ft_darray_append(minirt->objects, &new);
	create_plane(&new, (float[4]){0.0f, 0.1f, 0.0f, 0.0f});
	new.center = ft_calloc(4, sizeof(float));
	to_color(0x00ffffff, new.color);
	new.center[0] = 0.0f;
	new.center[1] = 5.0f;
	new.center[2] = -4.0f;
	new.center[3] = 1.0f;
	ft_darray_append(minirt->objects, &new);
	return (1);
}

int	screen_space(t_matrix **matrix, t_display *display)
{
	homogeneous_matrix(matrix, 3, 3);
	set_diagonal(*matrix, 1.0f);
	(*matrix)->points[0][0] = (float)display->width / 2.0f;
	(*matrix)->points[1][1] = (float)display->height / 2.0f;
	(*matrix)->points[2][2] = 0.0f;
	(*matrix)->points[2][3] = 0.0f;
	(*matrix)->points[3][3] = 0.0f;
	(*matrix)->points[2][2] = 0.0f;
	(*matrix)->points[2][3] = 0.0f;
	(*matrix)->points[3][3] = 0.0f;
	(*matrix)->points[0][3] = display->width / 2.0f + 0.5f;
	(*matrix)->points[1][3] = display->height / 2.0f + 0.5f; 
	(*matrix)->points[2][3] = 0.0f;
	(*matrix)->points[2][2] = 0.0f;
	return (0);
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
