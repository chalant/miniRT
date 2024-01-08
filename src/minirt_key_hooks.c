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

int	key_press_hook(int code, t_minirt *fdf)
{
	t_matrix	*tmp;
	float		vector[4];

	homogeneous_matrix(&tmp, 3, 3);
	vector[0] = 0.0f;
	vector[1] = 0.0f;
	vector[2] = 0.0f;
	vector[3] = 1.0f;
	if (code == RL)
	{
		inplace_matmul(fdf->rotations->y_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rev_rotations->y_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == RR)
	{
		inplace_matmul(fdf->rev_rotations->y_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rotations->y_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == RU)
	{
		inplace_matmul(fdf->rotations->x_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rev_rotations->x_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == RD)
	{
		inplace_matmul(fdf->rev_rotations->x_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rotations->x_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == TL)
	{
		inplace_matmul(fdf->translations->x_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rev_translations->x_axis, fdf->camera->inverse_transform, tmp);
		// vmatmul(fdf->translations->x_axis, fdf->camera->origin, vector);
		// fdf->camera->origin[0] = vector[0];
		// fdf->camera->origin[1] = vector[1];
		// fdf->camera->origin[2] = vector[2];
	}
	else if (code == TR)
	{
		inplace_matmul(fdf->rev_translations->x_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->translations->x_axis, fdf->camera->inverse_transform, tmp);
		// vmatmul(fdf->translations->x_axis, fdf->camera->origin, vector);
		// fdf->camera->origin[0] = vector[0];
		// fdf->camera->origin[1] = vector[1];
		// fdf->camera->origin[2] = vector[2];
	}
	else if (code == TU)
	{
		inplace_matmul(fdf->rev_translations->z_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->translations->z_axis, fdf->camera->inverse_transform, tmp);
		// vmatmul(fdf->rev_translations->z_axis, fdf->camera->origin, vector);
		// fdf->camera->origin[0] = vector[0];
		// fdf->camera->origin[1] = vector[1];
		// fdf->camera->origin[2] = vector[2];
		// fdf->camera->origin[3] = vector[3];
		fprintf(stderr, "%f\n", fdf->camera->origin[2]);
	}
	else if (code == TD)
	{
		inplace_matmul(fdf->translations->z_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rev_translations->z_axis, fdf->camera->inverse_transform, tmp);
		// vmatmul(fdf->translations->z_axis, fdf->camera->origin, vector);
		// fdf->camera->origin[0] = vector[0];
		// fdf->camera->origin[1] = vector[1];
		// fdf->camera->origin[2] = vector[2];
		// fdf->camera->origin[3] = vector[3];
		fprintf(stderr, "%f\n", fdf->camera->origin[2]);
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
