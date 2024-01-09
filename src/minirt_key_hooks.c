/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_key_hooks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/08 17:08:21 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	movement_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
}

int	color_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
	return (1);
}

void	fdf_control_key(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
}

void	print_matrix1(t_matrix *matrix)
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

// void	set_inverse_rotate_transforms(t_minirt *fdf, t_camera *camera,
// 		t_matrix *rev_transform)
// {
// 	t_matrix	*target;

// 	target = camera->inverse_transform;
// 	matmul(target, object->t_center, fdf->tmp);
// 	matrix_copy(fdf->tmp, target);
// 	matmul(target, rev_transform, fdf->tmp);
// 	matrix_copy(fdf->tmp, target);
// 	matmul(target, object->rev_t_center, fdf->tmp);
// 	matrix_copy(fdf->tmp, target);
// }

int	key_press_hook(int code, t_minirt *fdf)
{
	t_matrix	*tmp;

	//todo: use a cache for this.
	homogeneous_matrix(&tmp, 3, 3);
	if (code == RL)
	{
		inplace_matmul(fdf->rev_rotations->y_axis, fdf->camera->transform, tmp);
		//inplace_matmul(fdf->rotations->y_axis, fdf->camera->inverse_transform, tmp);
		matmul(fdf->camera->inverse_transform, fdf->rotations->y_axis, tmp);
		matrix_copy(tmp, fdf->camera->inverse_transform);
	}
	else if (code == RR)
	{
		inplace_matmul(fdf->rotations->y_axis, fdf->camera->transform, tmp);
		//inplace_matmul(fdf->rev_rotations->y_axis, fdf->camera->inverse_transform, tmp);
		matmul(fdf->camera->inverse_transform, fdf->rev_rotations->y_axis, tmp);
		matrix_copy(tmp, fdf->camera->inverse_transform);
	}
	else if (code == RU)
	{
		inplace_matmul(fdf->rev_rotations->x_axis, fdf->camera->transform, tmp);
		//inplace_matmul(fdf->rotations->x_axis, fdf->camera->inverse_transform, tmp);
		matmul(fdf->camera->inverse_transform, fdf->rotations->x_axis, tmp);
		matrix_copy(tmp, fdf->camera->inverse_transform);
	}
	else if (code == RD)
	{
		inplace_matmul(fdf->rotations->x_axis, fdf->camera->transform, tmp);
		//inplace_matmul(fdf->rev_rotations->x_axis, fdf->camera->inverse_transform, tmp);
		matmul(fdf->camera->inverse_transform, fdf->rev_rotations->x_axis, tmp);
		matrix_copy(tmp, fdf->camera->inverse_transform);
	}
	else if (code == TL)
	{
		inplace_matmul(fdf->rev_translations->x_axis, fdf->camera->transform, tmp);
		//inplace_matmul(fdf->rev_translations->x_axis, fdf->camera->inverse_transform, tmp);
		matmul(fdf->camera->inverse_transform, fdf->translations->x_axis, tmp);
		matrix_copy(tmp, fdf->camera->inverse_transform);
	}
	else if (code == TR)
	{
		inplace_matmul(fdf->translations->x_axis, fdf->camera->transform, tmp);
		//inplace_matmul(fdf->translations->x_axis, fdf->camera->inverse_transform, tmp);
		matmul(fdf->camera->inverse_transform, fdf->rev_translations->x_axis, tmp);
		matrix_copy(tmp, fdf->camera->inverse_transform);
	}
	else if (code == TU)
	{
		inplace_matmul(fdf->translations->z_axis, fdf->camera->transform, tmp);
		//inplace_matmul(fdf->translations->z_axis, fdf->camera->inverse_transform, tmp);
		matmul(fdf->camera->inverse_transform, fdf->rev_translations->z_axis, tmp);
		matrix_copy(tmp, fdf->camera->inverse_transform);
	}
	else if (code == TD)
	{
		inplace_matmul(fdf->rev_translations->z_axis, fdf->camera->transform, tmp);
		//inplace_matmul(fdf->rev_translations->z_axis, fdf->camera->inverse_transform, tmp);
		matmul(fdf->camera->inverse_transform, fdf->translations->z_axis, tmp);
		matrix_copy(tmp, fdf->camera->inverse_transform);
	}
	delete_matrix(tmp);
	return (0);
}

int	key_release_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
	return (0);
}
