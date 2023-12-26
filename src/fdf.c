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

#include "fdf.h"
#include <stdio.h>

int	set_fdf_transforms(t_fdf *fdf)
{
	if (!set_rotations(fdf, 5.1f, 5.1f, 5.1f))
		return (0);
	if (!set_translations(fdf, 0.1f, 0.1f, 0.1f))
		return (0);
	if (!set_scalings(fdf, 1.1f, 1.1f, 1.1f))
		return (0);
	if (!set_zooming(fdf, 1.1f))
		return (0);
	if (!create_perspective_mode(fdf))
		return (0);
	if (!create_isometric_mode(fdf))
		return (0);
	fdf->transforms = identity_matrix(3, 3);
	fdf->centering = translation(fdf->display->height / 2.0f,
			fdf->display->width / 2.0f, 0.0f);
	fdf->tmp = homogeneous_matrix(3, 3);
	fdf->mode = fdf->isometric_mode;
	return (1);
}

int	mlx_setup(t_fdf *fdf)
{
	t_display	*display;

	display = fdf->display;
	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		return (0);
	fdf->window = mlx_new_window(fdf->mlx, display->width, display->height,
			"FDF");
	if (!fdf->window)
		return (0);
	display->img = mlx_new_image(fdf->mlx, display->width, display->height);
	if (!display->img)
		return (0);
	display->addr = mlx_get_data_addr(display->img, &display->bits_per_pixel,
			&display->line_length, &display->endian);
	if (!display->addr)
		return (0);
	return (1);
}

int	set_hooks(t_fdf *fdf)
{
	mlx_hook(fdf->window, 2, 1L << 0, key_press_hook, fdf);
	mlx_hook(fdf->window, 3, 1L << 1, key_release_hook, fdf);
	mlx_hook(fdf->window, 17, 0, close_program, fdf);
	mlx_expose_hook(fdf->window, handle_expose, fdf);
	mlx_loop_hook(fdf->mlx, update_view, fdf);
	return (1);
}

int	set_variables(t_fdf *fdf)
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
	t_fdf		fdf;
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
	if (!set_fdf_transforms(&fdf))
		return (fdf_free(&fdf, 1));
	if (!normalize_object(&fdf, fdf.map))
		return (fdf_free(&fdf, 1));
	set_hooks(&fdf);
	mlx_loop(fdf.mlx);
	return (0);
}
