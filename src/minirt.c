/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                              :+:      :+:    :+:   */
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
	if (set_translations(minirt, 0.01f, 0.01f, 0.01f) < 0)
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
	//todo: malloc protection
	minirt->camera->fov = 60.0f;
	minirt->camera->near = -1.0f;
	minirt->camera->far = 1.0f;
	minirt->camera->orientation = ft_calloc(4, sizeof(float));
	minirt->camera->orientation[0] = 0.0f;
	minirt->camera->orientation[1] = 0.0f;
	minirt->camera->orientation[2] = 0.0f;
	minirt->camera->orientation[3] = 0.0f;
	// minirt->camera->t_origin;
	// minirt->camera->t_rev_origin;
	minirt->camera->origin = ft_calloc(3, sizeof(float));
	minirt->camera->origin[0] = 0.0f;
	minirt->camera->origin[1] = 0.0f;
	minirt->camera->origin[2] = 1.0f;
	minirt->camera->ray_direction = ft_calloc(4, sizeof(float));
	minirt->camera->ray_direction[3] = 0.0f;
	set_camera_transform(minirt->camera, minirt->display);
	create_sphere(&new, 0.5f);
	ft_darray_append(minirt->objects, &new);
	return (1);
}

void	print_matrix2(t_matrix *matrix)
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
	// (*matrix)->points[0][0] = 200.0f;
	// (*matrix)->points[1][1] = 200.0f;
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
	t_matrix	*tmp;


	tmp = malloc(sizeof(t_matrix));
	create_matrix(tmp, 3, 3);
	init_matrix(tmp, 0.0f);
	tmp->points[0][0] = 1.0f;
	tmp->points[0][1] = 2.0f;
	tmp->points[0][2] = 3.0f;
	tmp->points[1][0] = 4.0f;
	tmp->points[1][1] = 5.0f;
	tmp->points[1][2] = 6.0f;
	tmp->points[2][0] = 7.0f;
	tmp->points[2][1] = 8.0f;
	tmp->points[2][2] = 9.0f;

	print_matrix2(tmp);
	invert_matrix(tmp, 3);
	print_matrix2(tmp);

	(void)argv;
	if (argc - 1 < 0 || argc - 1 > 1)
		return (0);
	minirt_init(&minirt);
	minirt.display = &display;
	set_variables(&minirt);
	mlx_setup(&minirt);
	// set_minirt_transforms(&minirt);
	set_hooks(&minirt);
	set_minirt_transforms(&minirt);
	homogeneous_matrix(&minirt.tmp, 3, 3);
	load_scene(&minirt);
	perspective_projector(&minirt.world_space, minirt.camera);
	perspective_projector(&minirt.view_matrix, minirt.camera);
	print_matrix2(minirt.world_space);
	invert_matrix(minirt.world_space, 4);
	print_matrix2(minirt.world_space);
	screen_space(&minirt.screen_space, minirt.display);
	// perspective_projector(&minirt.screen_space, minirt.camera);
	mlx_loop(minirt.mlx);
	return (0);
}
