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

void	set_translate(t_matrix *matrix, float x, float y, float z)
{
	matrix->points[0][matrix->cols - 1] = x;
	matrix->points[1][matrix->cols - 1] = y;
	matrix->points[2][matrix->cols - 1] = z;
}

int	key_press_hook(int code, t_minirt *minirt)
{
	printf("Code %d\n", code);
	minirt->key_pressed = 1;
	minirt->render_mode = low_resolution;
	if (code == CTRL)
		minirt->ctrl.pressed = 1;
	else if (code == SHIFT)
		minirt->shift.pressed = 1;
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
