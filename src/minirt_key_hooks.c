/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_key_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/30 12:48:19 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	movement_hook(int code, t_minirt *fdf)
{
	if (fdf->space == fdf->world)
		translation_hook(code, fdf);
	else if (fdf->space == fdf->view)
		view_translation_hook(code, fdf);
	rotation_hook(code, fdf);
}

int	color_hook(int code, t_minirt *fdf)
{
	if (fdf->shift->pressed && code == RU)
		fdf_increment_limit(&fdf->target->light_base, 0.01f, 0.0f, 1.0f);
	else if (fdf->shift->pressed && code == RD)
		fdf_increment_limit(&fdf->target->light_base, -0.01f, 0.0f, 1.0f);
	else if (code == RU)
		fdf->map->intensity += 10.0f;
	else if (code == RD)
		fdf->map->intensity -= 10.0f;
	else if (code == RL)
		fdf->map->base_color -= 5.0f;
	else if (code == RR)
		fdf->map->base_color += 5.0f;
	else if (fdf->shift->pressed && code == ZI)
		fdf_increment_limit(&fdf->target->saturation, 0.01f, 0.0f, 1.0f);
	else if (fdf->shift->pressed && code == ZO)
		fdf_increment_limit(&fdf->target->saturation, -0.01f, 0.0f, 1.0f);
	else if (code == ZI)
		fdf_increment_limit(&fdf->target->lightness, 0.01f, 0.0f, 1.0f);
	else if (code == ZO)
		fdf_increment_limit(&fdf->target->lightness, -0.01f, 0.0f, 1.0f);
	else if (code == LIGHT)
		fdf->target->light = !fdf->target->light;
	return (1);
}

void	fdf_control_key(int code, t_minirt *fdf)
{
	if (code == CTRL)
		fdf->ctrl->pressed = 1;
	if (code == SHIFT)
		fdf->shift->pressed = 1;
	if (code == COMMAND)
		fdf->command->pressed = 1;
}

int	key_press_hook(int code, t_minirt *fdf)
{
	if (code == ESCAPE)
		return (close_program(fdf));
	fdf_control_key(code, fdf);
	if (fdf->command->pressed)
		return (color_hook(code, fdf));
	if (fdf->ctrl->pressed && code == MODE && fdf->mode == fdf->isometric_mode)
		fdf->mode = fdf->perspective_mode;
	else if (fdf->ctrl->pressed && code == MODE
		&& fdf->mode == fdf->perspective_mode)
		fdf->mode = fdf->isometric_mode;
	if (code == SPACE && fdf->space == fdf->view)
		fdf->space = fdf->world;
	else if (code == SPACE && fdf->space == fdf->world)
		fdf->space = fdf->view;
	if (fdf->ctrl->pressed)
		scaling_hook(code, fdf);
	else
		movement_hook(code, fdf);
	zooming_hook(code, fdf);
	return (0);
}

int	key_release_hook(int code, t_minirt *fdf)
{
	if (code == CTRL)
		fdf->ctrl->pressed = 0;
	if (code == SHIFT)
		fdf->shift->pressed = 0;
	if (code == COMMAND)
		fdf->command->pressed = 0;
	return (0);
}
