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

void	movement_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
}

void	fdf_control_key(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
}

void	set_translate(t_matrix *matrix, float x, float y, float z)
{
	matrix->points[0][matrix->cols - 1] = x;
	matrix->points[1][matrix->cols - 1] = y;
	matrix->points[2][matrix->cols - 1] = z;
}

void	print_matrix3(t_matrix *matrix)
{
	int	i;
	int	j;

	i = -1;
	while (++i < matrix->rows)
	{
		j = -1;
		while (++j < matrix->cols)
			fprintf(stderr, "%f ", matrix->points[i][j]);
		fprintf(stderr, "\n");
	}
}

int	rotate_camera(t_minirt *minirt, t_matrix *rot)
{
	float	origin[4];

	matmul(&minirt->camera.basis, rot, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &minirt->camera.basis, 3);
	vmatmul(&minirt->camera.basis, minirt->camera.origin, origin);
	look_at(&minirt->camera, minirt);
	return (0);
}

int	translate_camera(t_minirt *minirt, float direction[3])
{
	return (0);
}

int	key_press_hook(int code, t_minirt *minirt)
{
	float		tmp[4];
	// t_object	*object;

	if (code == CTRL)
		minirt->ctrl.pressed = 1;
	else if (code == SHIFT)
		minirt->shift.pressed = 1;
	// object = ft_darray_get(&fdf->objects, fdf->objects.size - 1);
	if (code == RL)
		rotate_camera(minirt, &minirt->rev_rotations->y_axis);
	else if (code == RR)
		rotate_camera(minirt, &minirt->rotations->y_axis);
	else if (code == RU)
		rotate_camera(minirt, &minirt->rev_rotations->x_axis);
	else if (code == RD)
		rotate_camera(minirt, &minirt->rotations->x_axis);
	else if (code == TL)
	{
		tmp[0] = 1.0f;
		tmp[1] = 0.0f;
		tmp[2] = 0.0f;
		//rotate(fdf, object, fdf->rotations->x_axis);
		vmatmul(&minirt->camera.basis, tmp, minirt->camera.orientation);
		add_vectors(minirt->camera.origin, scale_vector(minirt->camera.orientation, 0.1f,  minirt->camera.orientation, 3), minirt->camera.origin, 3);
		look_at(&minirt->camera, minirt);

	}
	else if (code == TR)
	{
		tmp[0] = -1.0f;
		tmp[1] = 0.0f;
		tmp[2] = 0.0f;
		vmatmul(&minirt->camera.basis, tmp, minirt->camera.orientation);
		add_vectors(minirt->camera.origin, scale_vector(minirt->camera.orientation, 0.1f,  minirt->camera.orientation, 3), minirt->camera.origin, 3);
		look_at(&minirt->camera, minirt);
	}
	else if (code == TU)
	{
		tmp[0] = 0.0f;
		tmp[1] = 0.0f;
		tmp[2] = 1.0f;
		vmatmul(&minirt->camera.basis, tmp, minirt->camera.orientation);
		add_vectors(minirt->camera.origin, scale_vector(minirt->camera.orientation, 0.1f,  minirt->camera.orientation, 3), minirt->camera.origin, 3);
		look_at(&minirt->camera, minirt);
	}
	else if (code == TD)
	{
		tmp[0] = 0.0f;
		tmp[1] = 0.0f;
		tmp[2] = -1.0f;
		vmatmul(&minirt->camera.basis, tmp, minirt->camera.orientation);
		add_vectors(minirt->camera.origin, scale_vector(minirt->camera.orientation, 0.1f,  minirt->camera.orientation, 3), minirt->camera.origin, 3);
		look_at(&minirt->camera, minirt);
	}
	return (0);
}

int	key_release_hook(int code, t_minirt *minirt)
{
	if (code == CTRL)
		minirt->ctrl.pressed = 0;
	else if (code == SHIFT)
		minirt->ctrl.pressed = 0;
	return (0);
}
