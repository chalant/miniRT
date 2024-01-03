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
#include <stdio.h>

int	set_minirt_transforms(t_minirt *minirt)
{
	if (!set_rotations(minirt, 5.1f, 5.1f, 5.1f))
		return (0);
	if (!set_translations(minirt, 0.1f, 0.1f, 0.1f))
		return (0);
	if (!set_scalings(minirt, 1.1f, 1.1f, 1.1f))
		return (0);
	if (!set_zooming(minirt, 1.1f))
		return (0);
	if (!create_perspective_mode(minirt))
		return (0);
	if (!create_isometric_mode(minirt))
		return (0);
	homogeneous_matrix(&minirt->transforms, 3, 3);
	identity_matrix(minirt->transforms, 3, 3);
	homogeneous_matrix(&minirt->centering, 3, 3);
	translation(minirt->centering, minirt->display->height / 2.0f,
		minirt->display->width / 2.0f, 0.0f);
	homogeneous_matrix(&minirt->tmp, 3, 3);
	return (1);
}

int	mlx_setup(t_minirt *minirt)
{
	t_display	*display;

	display = minirt->display;
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
	minirt->display->height = 1080;
	minirt->display->width = 1920;
	return (1);
}

int	main(int argc, char *argv[])
{
	t_minirt	minirt;
	t_display	display;

	if (argc - 1 < 0 || argc - 1 > 1)
		return (0);
	minirt_init(&minirt);
	minirt.display = &display;
	set_variables(&minirt);
	mlx_setup(&minirt);
	set_minirt_transforms(&minirt);
	set_hooks(&minirt);
	mlx_loop(minirt.mlx);
	return (0);
}
