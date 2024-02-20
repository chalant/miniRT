/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_key_hooks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:58:48 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	next_element(t_minirt *minirt)
{
	if (minirt->selected_object)
	{
		if (minirt->shift.pressed)
		{
			minirt->selected_object->texture_index++;
			minirt->selected_object->texture_index
				= minirt->selected_object->texture_index
				% minirt->textures.size;
		}
		else
		{
			minirt->selected_object->perturbator_index++;
			minirt->selected_object->perturbator_index
				= minirt->selected_object->perturbator_index
				% minirt->perturbators.size;
		}
	}
	minirt->light_index++;
	minirt->light_index = minirt->light_index % minirt->lights.size;
	return (0);
}

int	control_light(int code, t_minirt *minirt)
{
	t_light	*light;

	light = ft_darray_get(&minirt->lights, minirt->light_index);
	if (code == ROT_LEFT)
		light->position[0] -= 5.0f;
	else if (code == ROT_RIGHT)
		light->position[0] += 5.0f;
	else if (code == ROT_UP && !minirt->ctrl.pressed)
		light->position[2] += 5.0f;
	else if (code == ROT_DOWN && !minirt->ctrl.pressed)
		light->position[2] -= 5.0f;
	return (0);
}

int	key_press_hook(int code, t_minirt *minirt)
{
	if (code == ESCAPE)
		return (close_program(minirt, 0));
	minirt->key_pressed = 1;
	if (code == CTRL)
		minirt->ctrl.pressed = 1;
	else if (code == SHIFT)
		minirt->shift.pressed = 1;
	else if (code == ALT)
		minirt->alt.pressed = 1;
	else if (code == TAB)
		next_element(minirt);
	else if (!minirt->shift.pressed)
		control_camera(code, minirt);
	else if (minirt->shift.pressed)
		control_light(code, minirt);
	return (0);
}

int	key_release_hook(int code, t_minirt *minirt)
{
	if (!minirt->mouse.left_click)
		minirt->render_mode = minirt->high_res;
	if (code == CTRL)
		minirt->ctrl.pressed = 0;
	else if (code == SHIFT)
		minirt->shift.pressed = 0;
	else if (code == ALT)
		minirt->alt.pressed = 0;
	minirt->key_pressed = 0;
	return (0);
}
