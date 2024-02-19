/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_rotations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:19:47 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:19:48 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	rotate_object(t_minirt *minirt, t_object *object, t_matrix *axis)
{
	matrix_product(&object->basis, axis, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &object->basis, 3);
	return (0);
}

int	rotate_x(t_minirt *minirt, t_matrix *matrix, float dir[3])
{
	float	tmp[4];

	x_rotation(matrix, 5.0f * to_rad(minirt->mouse.hit_point * dir[1]));
	rotate_object(minirt, minirt->selected_object, matrix);
	matvec_product(matrix, minirt->selected_object->orientation, tmp);
	copy_vector(tmp, minirt->selected_object->orientation, 3);
	matrix->points[0][0] = 0.0f;
	matrix->points[1][1] = 0.0f;
	matrix->points[2][1] = 0.0f;
	matrix->points[1][2] = 0.0f;
	matrix->points[2][2] = 0.0f;
	return (1);
}

int	rotate_y(t_minirt *minirt, t_matrix *matrix, float dir[3])
{
	float	tmp[4];

	y_rotation(matrix, 5.0f * to_rad(minirt->mouse.hit_point * dir[0]));
	rotate_object(minirt, minirt->selected_object, matrix);
	matvec_product(matrix, minirt->selected_object->orientation, tmp);
	copy_vector(tmp, minirt->selected_object->orientation, 3);
	matrix->points[0][0] = 0.0f;
	matrix->points[0][2] = 0.0f;
	matrix->points[1][1] = 0.0f;
	matrix->points[2][0] = 0.0f;
	matrix->points[2][2] = 0.0f;
	return (1);
}

int	rotate_z(t_minirt *minirt, t_matrix *matrix, float dir[3])
{
	float	tmp[4];

	z_rotation(matrix, minirt->mouse.hit_point * dir[0]);
	rotate_object(minirt, minirt->selected_object, matrix);
	matvec_product(matrix, minirt->selected_object->orientation, tmp);
	copy_vector(tmp, minirt->selected_object->orientation, 3);
	matrix->points[0][0] = 0.0f;
	matrix->points[0][1] = 0.0f;
	matrix->points[1][0] = 0.0f;
	matrix->points[1][1] = 0.0f;
	matrix->points[2][2] = 0.0f;
	return (1);
}
