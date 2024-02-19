/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_transforms.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:06:44 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:13:55 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	apply_rotation(t_minirt *minirt, float dir[4])
{
	minirt->render_mode = minirt->low_res;
	rotate_x(minirt, &minirt->mouse.direction, dir);
	rotate_y(minirt, &minirt->mouse.direction, dir);
	if (minirt->shift.pressed)
		rotate_z(minirt, &minirt->mouse.direction, dir);
}

void	apply_scaling(t_minirt *minirt, float dir[4])
{
	t_object	*selected;

	minirt->render_mode = minirt->low_res;
	selected = minirt->selected_object;
	minirt->selected_object->size[0] += -selected->size[0] * dir[0];
	minirt->selected_object->size[1] += selected->size[1] * dir[1];
	minirt->selected_object->size[2] += dir[2];
}

void	apply_translation(t_minirt *minirt, float dir[4], float speed)
{
	minirt->render_mode = minirt->low_res;
	minirt->selected_object->center[0] += speed * dir[0];
	minirt->selected_object->center[1] += speed * dir[1];
	minirt->selected_object->center[2] += speed * dir[2];
}
