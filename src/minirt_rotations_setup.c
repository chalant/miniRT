/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_rotations_setup.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/26 17:10:00 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	to_rad(float angle)
{
	return (angle * (M_PI / 180.0));
}

int	x_rotation(t_matrix *matrix, float angle)
{
	matrix->points[0][0] = 1.0f;
	matrix->points[1][1] = cosf(angle);
	matrix->points[1][2] = -sinf(angle);
	matrix->points[2][1] = sinf(angle);
	matrix->points[2][2] = cosf(angle);
	return (0);
}

int	y_rotation(t_matrix *matrix, float angle)
{
	matrix->points[0][0] = cosf(angle);
	matrix->points[0][2] = sinf(angle);
	matrix->points[1][1] = 1.0f;
	matrix->points[2][0] = -sinf(angle);
	matrix->points[2][2] = cosf(angle);
	return (0);
}

int	z_rotation(t_matrix *matrix, float angle)
{
	matrix->points[0][0] = cosf(angle);
	matrix->points[0][1] = -sinf(angle);
	matrix->points[1][0] = sinf(angle);
	matrix->points[1][1] = cosf(angle);
	matrix->points[2][2] = 1.0f;
	return (0);
}

int	set_rotations(t_minirt *fdf, float x, float y, float z)
{
	if (homogeneous_matrix(&fdf->rotations.x_axis, 3, 3))
		return (-1);
	x_rotation(&fdf->rotations.x_axis, to_rad(x));
	if (homogeneous_matrix(&fdf->rotations.y_axis, 3, 3))
		return (-1);
	y_rotation(&fdf->rotations.y_axis, to_rad(y));
	if (homogeneous_matrix(&fdf->rotations.z_axis, 3, 3))
		return (-1);
	z_rotation(&fdf->rotations.z_axis, to_rad(z));
	if (homogeneous_matrix(&fdf->rev_rotations.x_axis, 3, 3))
		return (-1);
	x_rotation(&fdf->rev_rotations.x_axis, to_rad(-x));
	if (homogeneous_matrix(&fdf->rev_rotations.y_axis, 3, 3))
		return (-1);
	y_rotation(&fdf->rev_rotations.y_axis, to_rad(-y));
	if (homogeneous_matrix(&fdf->rev_rotations.z_axis, 3, 3))
		return (-1);
	z_rotation(&fdf->rev_rotations.z_axis, to_rad(-z));
	return (1);
}
