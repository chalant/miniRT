/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:54:57 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "minirt_objects.h"

int	mlx_setup(t_minirt *minirt)
{
	t_display	*display;

	display = &minirt->display;
	display->height = 600;
	display->width = 800;
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

int	set_hooks(t_minirt *minirt)
{
	mlx_hook(minirt->window, 6, 1L << 6, mouse_update, minirt);
	mlx_hook(minirt->window, 4, 1L << 2, mouse_click_hook, minirt);
	mlx_hook(minirt->window, 5, 1L << 3, mouse_release_hook, minirt);
	mlx_hook(minirt->window, 2, 1L << 0, key_press_hook, minirt);
	mlx_hook(minirt->window, 3, 1L << 1, key_release_hook, minirt);
	mlx_hook(minirt->window, 17, 0, close_program, minirt);
	mlx_loop_hook(minirt->mlx, render, minirt);
	return (1);
}

int	minirt_zero(t_minirt *minirt)
{
	minirt->mlx = NULL;
	minirt->window = NULL;
	minirt->selected_object = NULL;
	minirt->mouse.x = 0;
	minirt->mouse.y = 0;
	minirt->mouse.left_click = 0;
	minirt->ctrl.pressed = 0;
	minirt->shift.pressed = 0;
	minirt->key_pressed = 0;
	minirt->light_index = 0;
	minirt->alt.pressed = 0;
	minirt->mouse.direction.points = NULL;
	minirt->objects.contents = NULL;
	minirt->materials.contents = NULL;
	minirt->lights.contents = NULL;
	minirt->perturbators.contents = NULL;
	minirt->textures.contents = NULL;
	minirt->camera.basis.points = NULL;
	minirt->camera.inverse_view.points = NULL;
	minirt->camera.view.points = NULL;
	return (0);
}

int	minirt_allocate(t_minirt *minirt)
{
	if (set_rotations(minirt, 2.1f, 2.1f, 2.1f) < 0)
		return (-1);
	if (homogeneous_matrix(&minirt->mouse.direction, 3, 3))
		return (-1);
	if (ft_darray_init(&minirt->objects, sizeof(t_object), 10) < 0)
		return (-1);
	if (ft_darray_init(&minirt->materials, sizeof(t_material), 10) < 0)
		return (-1);
	if (ft_darray_init(&minirt->lights, sizeof(t_light), 10) < 0)
		return (-1);
	if (ft_darray_init(&minirt->perturbators, sizeof(t_perturbator), 10) < 0)
		return (-1);
	if (ft_darray_init(&minirt->textures, sizeof(t_texture), 10) < 0)
		return (-1);
	if (homogeneous_matrix(&minirt->tmp, 3, 3))
		return (-1);
	return (0);
}

int	minirt_init(t_minirt *minirt)
{
	minirt_zero(minirt);
	if (minirt_allocate(minirt))
		return (-1);
	mlx_setup(minirt);
	set_hooks(minirt);
	to_color(0x0087ceeb, minirt->sky_color);
	minirt->low_res = low_resolution;
	minirt->high_res = full_resolution;
	minirt->render_mode = minirt->high_res;
	return (1);
}
