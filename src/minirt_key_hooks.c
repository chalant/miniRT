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

void	set_translate(t_matrix *matrix, float x, float y, float z)
{
	matrix->points[0][matrix->cols - 1] = x;
	matrix->points[1][matrix->cols - 1] = y;
	matrix->points[2][matrix->cols - 1] = z;
}

int	rotate_around(t_minirt *fdf, t_matrix *rot)
{
	t_matrix	*tmp;

	//todo: use a cache for this.
	homogeneous_matrix(&tmp, 3, 3);
	matmul(fdf->camera->transform, rot, tmp, 4);
	matrix_copy(tmp, fdf->camera->transform, 4);
	//matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
	invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, tmp, 4);
	return (0);
}

int	key_press_hook(int code, t_minirt *fdf)
{
	t_matrix	*tmp;

	//todo: use a cache for this.
	homogeneous_matrix(&tmp, 3, 3);
	if (code == RL)
	{
		rotate_around(fdf, fdf->rotations->y_axis);
	}
	else if (code == RR)
	{
		rotate_around(fdf, fdf->rev_rotations->y_axis);
	}
	else if (code == RU)
	{
		rotate_around(fdf, fdf->rotations->x_axis);
	}
	else if (code == RD)
	{
		rotate_around(fdf, fdf->rev_rotations->x_axis);
	}
	else if (code == TL)
	{
		matmul(fdf->camera->transform, fdf->rev_translations->x_axis, tmp, 4);
		matrix_copy(tmp, fdf->camera->transform, 4);
		//matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
		invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, tmp, 4);
	}
	else if (code == TR)
	{
		matmul(fdf->camera->transform, fdf->translations->x_axis,tmp, 4);
		matrix_copy(tmp, fdf->camera->transform, 4);
		//matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
		invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, tmp, 4);
	}
	else if (code == TU)
	{
		matmul(fdf->camera->transform, fdf->rev_translations->z_axis, tmp, 4);
		matrix_copy(tmp, fdf->camera->transform, 4);
		//matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
		invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, tmp, 4);
	}
	else if (code == TD)
	{
		matmul(fdf->camera->transform, fdf->translations->z_axis, tmp, 4);
		matrix_copy(tmp, fdf->camera->transform, 4);
		//matrix_copy(fdf->camera->transform, fdf->camera->inverse_transform, 4);
		invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, tmp, 4);
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
