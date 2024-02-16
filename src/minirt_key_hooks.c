/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_key_hooks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/09 11:59:35 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	rotate_camera(t_minirt *minirt, t_matrix *rot)
{
	matrix_product(&minirt->camera.basis, rot, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &minirt->camera.basis, 3);
	look_at(&minirt->camera, minirt);
	return (0);
}

int	translate_camera(t_minirt *minirt, float direction[3])
{
	float	orientation[4];

	matvec_product(&minirt->camera.basis, direction, orientation);
	scale_vector(orientation, 0.5f, orientation, 3);
	if (minirt->selected_object)
		apply_translation(minirt, orientation, 1.0f);
	add_vectors(minirt->camera.origin,
		orientation, minirt->camera.origin, 3);
	look_at(&minirt->camera, minirt);
	return (0);
}

int	next_element(t_minirt *minirt)
{
	//todo: cycle colors (hsl) when shift is pressed
	if (minirt->selected_object)
	{
		//todo: cycle textures.
		if (minirt->shift.pressed)
		{
			minirt->selected_object->texture_index++;
			minirt->selected_object->texture_index = minirt->selected_object->texture_index
				% minirt->textures.size;
		}
		else
		{
			minirt->selected_object->perturbator_index++;
			minirt->selected_object->perturbator_index = minirt->selected_object->perturbator_index
				% minirt->perturbators.size;
		}
	}
	minirt->light_index++;
	minirt->light_index = minirt->light_index % minirt->spot_lights.size;
	return (0);
}

int	control_camera(int code, t_minirt *minirt)
{
	if (code == RL)
		rotate_camera(minirt, &minirt->rev_rotations.y_axis);
	else if (code == RR)
		rotate_camera(minirt, &minirt->rotations.y_axis);
	else if (code == RU)
		rotate_camera(minirt, &minirt->rev_rotations.x_axis);
	else if (code == RD)
		rotate_camera(minirt, &minirt->rotations.x_axis);
	else if (code == TL)
		translate_camera(minirt, (float [3]){1.0f, 0.0f, 0.0f});
	else if (code == TR)
		translate_camera(minirt, (float [3]){-1.0f, 0.0f, 0.0f});
	else if (code == TU)
		translate_camera(minirt, (float [3]){0.0f, 0.0f, 1.0f});
	else if (code == TD)
		translate_camera(minirt, (float [3]){0.0f, 0.0f, -1.0f});
	return (0);
}

int	control_light(int code, t_minirt *minirt)
{
	//todo: need camera orientation.
	//todo: hue control.
	t_light	*light;

	light = ft_darray_get(&minirt->spot_lights, minirt->light_index);
	if (code == RL)
		light->position[0] -= 5.0f;
	else if (code == RR)
		light->position[0] += 5.0f;
	else if (code == RU)
		light->position[2] += 5.0f;
	else if (code == RD)
		light->position[2] -= 5.0f;
	return (0);
}

int	key_press_hook(int code, t_minirt *minirt)
{
	minirt->key_pressed = 1;
	if (code != TAB)
		minirt->render_mode = low_resolution;
	if (code == CTRL)
		minirt->ctrl.pressed = 1;
	else if (code == SHIFT)
		minirt->shift.pressed = 1;
	else if (code == TAB)
		next_element(minirt);
	else if (!minirt->shift.pressed)
		control_camera(code, minirt);
	else
		control_light(code, minirt);
	return (0);
}

int	key_release_hook(int code, t_minirt *minirt)
{
	if (!minirt->mouse.left_click)
		minirt->render_mode = full_resolution;
	if (code == CTRL)
		minirt->ctrl.pressed = 0;
	else if (code == SHIFT)
		minirt->shift.pressed = 0;
	minirt->key_pressed = 0;
	return (0);
}
