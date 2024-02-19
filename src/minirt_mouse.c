/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:06:20 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:19:21 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	mouse_click_hook(int button, int x, int y, t_minirt *minirt)
{
	if (button == 1)
	{
		grab_object(minirt, x, y);
		minirt->mouse.left_click = 1;
	}
	minirt->mouse.x = x;
	minirt->mouse.y = y;
	return (0);
}

int	mouse_release_hook(int button, int x, int y, t_minirt *minirt)
{
	(void)x;
	(void)y;
	if (!minirt->key_pressed)
		minirt->render_mode = full_resolution;
	if (button == 1)
	{
		minirt->selected_object = NULL;
		minirt->mouse.left_click = 0;
	}
	return (0);
}

int	mouse_update(int x, int y, t_minirt *minirt)
{
	float	direction[4];
	float	dir[4];
	float	coords[4];
	float	position[4];

	if (!minirt->selected_object)
		return (0);
	to_screen_space(&minirt->display, coords, x, y);
	to_world_space(minirt, coords, position);
	subtract_vectors(position, minirt->mouse.world_position, direction, 3);
	direction[3] = 1.0f;
	matvec_product(&minirt->camera.basis, direction, dir);
	if (minirt->alt.pressed)
		update_material(minirt, minirt->selected_object, dir);
	else if (minirt->ctrl.pressed)
		apply_rotation(minirt, dir);
	else if (minirt->shift.pressed)
		apply_scaling(minirt, dir);
	else
		apply_translation(minirt, dir, minirt->mouse.hit_point);
	copy_vector(position, minirt->mouse.world_position, 3);
	return (0);
}
