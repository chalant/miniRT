/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
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
	translation(minirt->centering, minirt->display->height / 2.0f, minirt->display->width / 2.0f, 0.0f);
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
	minirt->window = mlx_new_window(minirt->mlx, display->width, display->height,
			"miniRT");
	if (!minirt->window)
		return (0);
	display->img = mlx_new_image(minirt->mlx, display->width, display->height);
	if (!display->img)
		return (0);
	display->addr = mlx_get_data_addr(display->img, &display->bits_per_pixel,
			&display->line_length, &display->endian);
	if (!display->addr)
		return (0);
	return (1);
}

int	set_hooks(t_minirt *fdf)
{
	mlx_hook(fdf->window, 2, 1L << 0, key_press_hook, fdf);
	mlx_hook(fdf->window, 3, 1L << 1, key_release_hook, fdf);
	mlx_hook(fdf->window, 17, 0, close_program, fdf);
	mlx_expose_hook(fdf->window, handle_expose, fdf);
	mlx_loop_hook(fdf->mlx, update_view, fdf);
	return (1);
}

int	set_variables(t_minirt *fdf)
{
	fdf->ctrl = malloc(sizeof(t_fdf_key));
	if (!fdf->ctrl)
		return (0);
	fdf->shift = malloc(sizeof(t_fdf_key));
	if (!fdf->shift)
		return (0);
	fdf->command = malloc(sizeof(t_fdf_key));
	if (!fdf->command)
		return (0);
	fdf->ctrl->pressed = 0;
	fdf->command->pressed = 0;
	fdf->shift->pressed = 0;
	fdf->display->height = 1080;
	fdf->display->width = 1920;
	fdf->world = WORLD;
	fdf->view = VIEW;
	fdf->space = fdf->world;
	return (1);
}

int	main(int argc, char *argv[])
{
	t_minirt	fdf;
	t_display	display;

	if (argc - 1 < 0 || argc - 1 > 1)
		return (0);
	fdf_init(&fdf);
	fdf.map = load_object(argv[1]);
	if (!fdf.map)
		return (fdf_free(&fdf, 1));
	fdf_object_color(fdf.map);
	fdf.display = &display;
	if (!set_variables(&fdf))
		return (fdf_free(&fdf, 1));
	if (!mlx_setup(&fdf))
		return (fdf_free(&fdf, 1));
	if (!set_minirt_transforms(&fdf))
		return (fdf_free(&fdf, 1));
	if (!normalize_object(&fdf, fdf.map))
		return (fdf_free(&fdf, 1));
	set_hooks(&fdf);
	mlx_loop(fdf.mlx);
	return (0);
}
