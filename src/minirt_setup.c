/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_setup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:06:41 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// int	set_transforms(t_transforms_3d *transforms, t_matrix *x, t_matrix *y,
// 		t_matrix *z)
// {
// 	if (!transforms || !x || !y || !z)
// 		return (0);
// 	transforms->x_axis = x;
// 	transforms->y_axis = y;
// 	transforms->z_axis = z;
// 	return (1);
// }

float	to_rad(float angle)
{
	return (angle * (M_PI / 180.0));
}

//todo: malloc protections.
int	set_rotations(t_minirt *fdf, float x, float y, float z)
{
	fdf->rotations = malloc(sizeof(t_transforms_3d));
	if (!fdf->rotations)
		return (0);
	fdf->rev_rotations = malloc(sizeof(t_transforms_3d));
	if (!fdf->rev_rotations)
		return (0);
	homogeneous_matrix(&fdf->rotations->x_axis, 3, 3);
	x_rotation(&fdf->rotations->x_axis, to_rad(x));
	homogeneous_matrix(&fdf->rotations->y_axis, 3, 3);
	y_rotation(&fdf->rotations->y_axis, to_rad(y));
	homogeneous_matrix(&fdf->rotations->z_axis, 3, 3);
	z_rotation(&fdf->rotations->z_axis, to_rad(z));
	homogeneous_matrix(&fdf->rev_rotations->x_axis, 3, 3);
	x_rotation(&fdf->rev_rotations->x_axis, to_rad(-x));
	homogeneous_matrix(&fdf->rev_rotations->y_axis, 3, 3);
	y_rotation(&fdf->rev_rotations->y_axis, to_rad(-y));
	homogeneous_matrix(&fdf->rev_rotations->z_axis, 3, 3);
	z_rotation(&fdf->rev_rotations->z_axis, to_rad(-z));
	return (1);
}

int	set_translations(t_minirt *fdf, float x, float y, float z)
{
	fdf->translations = malloc(sizeof(t_transforms_3d));
	if (!fdf->translations)
		return (0);
	fdf->rev_translations = malloc(sizeof(t_transforms_3d));
	if (!fdf->rev_translations)
		return (0);
	homogeneous_matrix(&fdf->translations->x_axis, 3, 3);
	translation(&fdf->translations->x_axis, x, 0.0f, 0.0f);
	homogeneous_matrix(&fdf->translations->y_axis, 3, 3);
	translation(&fdf->translations->y_axis, 0.0f, y, 0.0f);
	homogeneous_matrix(&fdf->translations->z_axis, 3, 3);
	translation(&fdf->translations->z_axis, 0.0f, 0.0f, z);
	homogeneous_matrix(&fdf->rev_translations->x_axis, 3, 3);
	translation(&fdf->rev_translations->x_axis, -x, 0.0f, 0.0f);
	homogeneous_matrix(&fdf->rev_translations->y_axis, 3, 3);
	translation(&fdf->rev_translations->y_axis, 0.0f, -y, 0.0f);
	homogeneous_matrix(&fdf->rev_translations->z_axis, 3, 3);
	translation(&fdf->rev_translations->z_axis, 0.0f, 0.0f, -z);
	return (1);
}

// int	set_scalings(t_minirt *fdf, float x, float y, float z)
// {
// 	fdf->scalings = malloc(sizeof(t_transforms_3d));
// 	if (!fdf->scalings)
// 		return (0);
// 	fdf->rev_scalings = malloc(sizeof(t_transforms_3d));
// 	if (!fdf->rev_scalings)
// 		return (0);
// 	if (!set_transforms(fdf->scalings, scaling(x, 1.0f, 1.0f), scaling(1.0f, y,
// 				1.0f), scaling(1.0f, 1.0f, z)))
// 		return (0);
// 	if (!set_transforms(fdf->rev_scalings, scaling(1.0f / x, 1.0f, 1.0f),
// 			scaling(1.0f, 1.0f / y, 1.0f), scaling(1.0f, 1.0f, 1.0f / z)))
// 		return (0);
// 	return (1);
// }

// int	set_zooming(t_minirt *fdf, float value)
// {
// 	fdf->zooming_in = scaling(value, value, value);
// 	fdf->zooming_out = scaling(1.0f / value, 1.0f / value, 1.0f / value);
// 	return (1);
// }
