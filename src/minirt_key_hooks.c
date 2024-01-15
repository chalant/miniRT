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

void	set_translate(t_matrix *matrix, float x, float y, float z)
{
	matrix->points[0][matrix->cols - 1] = x;
	matrix->points[1][matrix->cols - 1] = y;
	matrix->points[2][matrix->cols - 1] = z;
}

int	rotate_around(t_minirt *fdf, t_matrix *rot, t_matrix *rev_rot)
{
	// t_matrix	*rev;
	// t_matrix	*trans;
	t_matrix	*tmp;

	(void)rev_rot;
	//todo: use a cache for this.
	homogeneous_matrix(&tmp, 3, 3);
	// homogeneous_matrix(&rev, 3, 3);
	// homogeneous_matrix(&trans, 3, 3);
	//set_diagonal(rev, 1.0f);
	// set_diagonal(trans, 1.0f);
	// set_translate(rev, -fdf->camera->transform->points[0][3], -fdf->camera->transform->points[1][3], -fdf->camera->transform->points[2][3]);
	//set_translate(trans, fdf->camera->transform->points[0][3], fdf->camera->transform->points[1][3], fdf->camera->transform->points[2][3]);
	//matmul(fdf->camera->transform, rev, tmp, 4);
	//matrix_copy(tmp, fdf->camera->transform, 4);
	//fprintf(stderr, "%f %f %f\n", fdf->camera->transform->points[0][3], fdf->camera->transform->points[1][3], fdf->camera->transform->points[2][3]);
	matmul(fdf->camera->transform, rot, tmp, 4);
	matrix_copy(tmp, fdf->camera->transform, 4);
	//matmul(fdf->camera->transform, trans, tmp, 4);
	//matrix_copy(tmp, fdf->camera->transform, 4);
	// inplace_matmul(rev, fdf->camera->transform, tmp, 4);
	// inplace_matmul(rot, fdf->camera->transform,tmp, 4);
	// inplace_matmul(trans, fdf->camera->transform, tmp, 4);
	
	matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
	invert_matrix(fdf->camera->inverse_transform, 4);
	return (0);
}

int	key_press_hook(int code, t_minirt *fdf)
{
	t_matrix	*tmp;

	//todo: use a cache for this.
	homogeneous_matrix(&tmp, 3, 3);
	if (code == RL)
	{
		rotate_around(fdf, fdf->rotations->y_axis, fdf->rev_rotations->y_axis);
	}
	else if (code == RR)
	{
		rotate_around(fdf, fdf->rev_rotations->y_axis, fdf->rotations->y_axis);
	}
	else if (code == RU)
	{
		rotate_around(fdf, fdf->rotations->x_axis, fdf->rev_rotations->x_axis);
	}
	else if (code == RD)
	{
		rotate_around(fdf, fdf->rev_rotations->x_axis, fdf->rotations->x_axis);
	}
	else if (code == TL)
	{
		matmul(fdf->camera->transform, fdf->translations->x_axis, tmp, 4);
		matrix_copy(tmp, fdf->camera->transform, 4);
		matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
		invert_matrix(fdf->camera->inverse_transform, 4);
		// matmul(fdf->camera->inverse_transform, fdf->translations->x_axis, tmp, 4);
		// matrix_copy(tmp, fdf->camera->inverse_transform, 4);
	}
	else if (code == TR)
	{
		matmul(fdf->camera->transform, fdf->rev_translations->x_axis,tmp, 4);
		matrix_copy(tmp, fdf->camera->transform, 4);
		matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
		invert_matrix(fdf->camera->inverse_transform, 4);
		// matrix_copy(tmp, fdf->camera->transform, 4);
		// matmul(fdf->camera->inverse_transform, fdf->rev_translations->x_axis, tmp, 4);
		// matrix_copy(tmp, fdf->camera->inverse_transform, 4);
		//inplace_matmul(fdf->translations->x_axis, fdf->camera->inverse_transform, tmp, 4);
		//matmul(fdf->camera->inverse_transform, fdf->rev_translations->x_axis, tmp, 4);
		//matrix_copy(tmp, fdf->camera->inverse_transform, 4);
	}
	else if (code == TU)
	{
		//inplace_matmul(fdf->rev_translations->z_axis, fdf->camera->transform, tmp, 4);
		matmul(fdf->camera->transform, fdf->rev_translations->z_axis, tmp, 4);
		matrix_copy(tmp, fdf->camera->transform, 4);
		matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
		invert_matrix(fdf->camera->inverse_transform, 4);
		// matmul(fdf->camera->inverse_transform, fdf->rev_translations->z_axis, tmp, 4);
		// matrix_copy(tmp, fdf->camera->inverse_transform, 4);
		// //inplace_matmul(fdf->translations->z_axis, fdf->camera->inverse_transform, tmp, 4);
		// fprintf(stderr, "%f\n", fdf->camera->transform->points[2][3]);
		//matmul(fdf->camera->inverse_transform, fdf->translations->z_axis, tmp, 4);
		//matrix_copy(tmp, fdf->camera->inverse_transform, 4);
	}
	else if (code == TD)
	{
		matmul(fdf->camera->transform, fdf->translations->z_axis, tmp, 4);
		matrix_copy(tmp, fdf->camera->transform, 4);
		matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
		invert_matrix(fdf->camera->inverse_transform, 4);
		// matmul(fdf->camera->inverse_transform, fdf->rev_translations->z_axis, tmp, 4);
		// matrix_copy(tmp, fdf->camera->inverse_transform, 4);
		// //inplace_matmul(fdf->rev_translations->z_axis, fdf->camera->inverse_transform, tmp, 4);
		// fprintf(stderr, "%f\n", fdf->camera->transform->points[2][3]);
		//matmul(fdf->camera->inverse_transform, fdf->rev_translations->z_axis, tmp, 4);
		//matrix_copy(tmp, fdf->camera->inverse_transform, 4);
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
