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

int	rotate_around(t_minirt *fdf, t_matrix *rot)
{
	matmul(fdf->camera->transform, rot, fdf->tmp, 4);
	matrix_copy(fdf->tmp, fdf->camera->transform, 4);
	invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, fdf->tmp, 4);
	return (0);
}

int	key_press_hook(int code, t_minirt *fdf)
{
	if (code == RL)
	{
		rotate_around(fdf, fdf->rotations->y_axis);	
		//rotate_around(fdf, fdf->rotations->y_axis);
	}
	else if (code == RR)
	{
		rotate_around(fdf, fdf->rev_rotations->y_axis);
		//rotate_around(fdf, fdf->rev_rotations->y_axis);
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
		//rotate_around(fdf, fdf->rotations->z_axis);	
		matmul(fdf->camera->transform, fdf->rev_translations->x_axis, fdf->tmp, 4);
		matrix_copy(fdf->tmp, fdf->camera->transform, 4);
		invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, fdf->tmp, 4);
	}
	else if (code == TR)
	{
		//rotate_around(fdf, fdf->rev_rotations->z_axis);	
		matmul(fdf->camera->transform, fdf->translations->x_axis, fdf->tmp, 4);
		matrix_copy(fdf->tmp, fdf->camera->transform, 4);
		invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, fdf->tmp, 4);
	}
	else if (code == TU)
	{
		matmul(fdf->camera->transform, fdf->rev_translations->z_axis, fdf->tmp, 4);
		matrix_copy(fdf->tmp, fdf->camera->transform, 4);
		invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, fdf->tmp, 4);
	}
	else if (code == TD)
	{
		matmul(fdf->camera->transform, fdf->translations->z_axis, fdf->tmp, 4);
		matrix_copy(fdf->tmp, fdf->camera->transform, 4);
		invert_matrix(fdf->camera->transform, fdf->camera->inverse_transform, fdf->tmp, 4);
	}
	return (0);
}

int	key_release_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
	return (0);
}
