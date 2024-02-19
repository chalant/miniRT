/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_camera_control.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:59:10 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:59:14 by ychalant         ###   ########.fr       */
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
	add_vectors(minirt->camera.position,
		orientation, minirt->camera.position, 3);
	look_at(&minirt->camera, minirt);
	return (0);
}

int	control_camera(int code, t_minirt *minirt)
{
	minirt->render_mode = minirt->low_res;
	if (code == ROT_LEFT)
		rotate_camera(minirt, &minirt->rev_rotations.y_axis);
	else if (code == ROT_RIGHT)
		rotate_camera(minirt, &minirt->rotations.y_axis);
	else if (code == ROT_UP)
		rotate_camera(minirt, &minirt->rev_rotations.x_axis);
	else if (code == ROT_DOWN)
		rotate_camera(minirt, &minirt->rotations.x_axis);
	else if (code == TILT_RIGHT)
		rotate_camera(minirt, &minirt->rotations.z_axis);
	else if (code == TILT_LEFT)
		rotate_camera(minirt, &minirt->rev_rotations.z_axis);
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
