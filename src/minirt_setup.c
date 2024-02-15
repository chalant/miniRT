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

float	to_rad(float angle)
{
	return (angle * (M_PI / 180.0));
}

//todo: malloc protections.
int	set_rotations(t_minirt *fdf, float x, float y, float z)
{
	homogeneous_matrix(&fdf->rotations.x_axis, 3, 3);
	x_rotation(&fdf->rotations.x_axis, to_rad(x));
	homogeneous_matrix(&fdf->rotations.y_axis, 3, 3);
	y_rotation(&fdf->rotations.y_axis, to_rad(y));
	homogeneous_matrix(&fdf->rotations.z_axis, 3, 3);
	z_rotation(&fdf->rotations.z_axis, to_rad(z));
	homogeneous_matrix(&fdf->rev_rotations.x_axis, 3, 3);
	x_rotation(&fdf->rev_rotations.x_axis, to_rad(-x));
	homogeneous_matrix(&fdf->rev_rotations.y_axis, 3, 3);
	y_rotation(&fdf->rev_rotations.y_axis, to_rad(-y));
	homogeneous_matrix(&fdf->rev_rotations.z_axis, 3, 3);
	z_rotation(&fdf->rev_rotations.z_axis, to_rad(-z));
	return (1);
}
