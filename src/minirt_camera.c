/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:05:24 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:53:07 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_translate(t_matrix *matrix, float x, float y, float z)
{
	matrix->points[0][matrix->cols - 1] = x;
	matrix->points[1][matrix->cols - 1] = y;
	matrix->points[2][matrix->cols - 1] = z;
}

int	set_col(t_matrix *matrix, float *values, int col, int n)
{
	while (--n > -1)
		matrix->points[n][col] = values[n];
	return (0);
}

int	look_at(t_camera *camera, t_minirt *fdf)
{
	invert_matrix(&camera->basis, &camera->view, &fdf->tmp, 3);
	set_translate(&camera->view, -camera->position[0],
		-camera->position[1], -camera->position[2]);
	matrix_copy(&camera->basis, &camera->inverse_view, 3);
	set_translate(&camera->inverse_view, camera->position[0],
		camera->position[1], camera->position[2]);
	return (0);
}

int	set_basis(t_matrix *basis, float orientation[3])
{
	float		up[3];
	float		right[3];

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;
	normalize_vector(orientation, orientation, 3);
	if (fabsf(dot_product(orientation, up, 3)) == 1.0f)
	{
		up[0] = 0.0f;
		up[1] = 0.0f;
		up[2] = 1.0f;
	}
	cross_product(up, orientation, right);
	normalize_vector(right, right, 3);
	cross_product(orientation, right, up);
	normalize_vector(up, up, 3);
	set_col(basis, right, 0, 3);
	set_col(basis, up, 1, 3);
	set_col(basis, orientation, 2, 3);
	return (0);
}

int	set_camera_transform(t_minirt *minirt, t_camera *camera)
{
	camera->near = -1.0f;
	camera->far = 1.0f;
	normalize_vector(camera->orientation, camera->orientation, 3);
	if (homogeneous_matrix(&camera->basis, 2, 2) == -1)
		return (-1);
	if (homogeneous_matrix(&camera->inverse_view, 3, 3) == -1)
		return (-1);
	if (homogeneous_matrix(&camera->view, 3, 3) == -1)
		return (-1);
	camera->position = camera->origin;
	if (perspective_projector(&minirt->world_space,
			&minirt->display, camera))
		return (-1);
	invert_matrix(&minirt->world_space, &minirt->world_space, &minirt->tmp, 4);
	set_basis(&camera->basis, camera->orientation);
	look_at(camera, minirt);
	return (0);
}
