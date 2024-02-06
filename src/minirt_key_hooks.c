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

int	rotate_around(t_minirt *fdf, t_matrix *rot)
{
	matmul(fdf->camera.basis, rot, fdf->tmp, 3);
	matrix_copy(fdf->tmp, fdf->camera.basis, 3);
	return (0);
}

int	key_press_hook(int code, t_minirt *fdf)
{
	float		tmp[4];
	// t_object	*object;

	// object = ft_darray_get(&fdf->objects, fdf->objects.size - 1);
	if (code == RL)
	{
		// if (!fdf->mouse.capture)
		// {
		// 	mlx_mouse_hide();
		// 	fdf->mouse.capture = 1;
		// }
		// else
		// {
		// 	mlx_mouse_show();
		// 	fdf->mouse.capture = 0;
		// }
		rotate_around(fdf, fdf->rev_rotations->y_axis);
		look_at(&fdf->camera, fdf);
	}
	else if (code == RR)
	{
		rotate_around(fdf, fdf->rotations->y_axis);
		look_at(&fdf->camera, fdf);

	}
	else if (code == RU)
	{
		// fdf->camera->basis->points[0][0] = -1.0f;
		// fdf->camera->basis->points[1][0] = 0.0f;
		// fdf->camera->basis->points[2][0] = 0.0f;
		// normalize_vector(fdf->camera->basis->points[0], 3);
		rotate_around(fdf, fdf->rev_rotations->x_axis);
		look_at(&fdf->camera, fdf);
	}
	else if (code == RD)
	{
		rotate_around(fdf, fdf->rotations->x_axis);
		look_at(&fdf->camera, fdf);
	}
	
	else if (code == TL)
	{
		tmp[0] = 1.0f;
		tmp[1] = 0.0f;
		tmp[2] = 0.0f;
		//rotate(fdf, object, fdf->rotations->x_axis);
		vmatmul(fdf->camera.basis, tmp, fdf->camera.orientation);
		add_vectors(fdf->camera.origin, scale_vector(fdf->camera.orientation, 0.1f,  fdf->camera.orientation, 3), fdf->camera.origin, 3);
		look_at(&fdf->camera, fdf);

	}
	else if (code == TR)
	{
		tmp[0] = -1.0f;
		tmp[1] = 0.0f;
		tmp[2] = 0.0f;
		vmatmul(fdf->camera.basis, tmp, fdf->camera.orientation);
		add_vectors(fdf->camera.origin, scale_vector(fdf->camera.orientation, 0.1f,  fdf->camera.orientation, 3), fdf->camera.origin, 3);
		look_at(&fdf->camera, fdf);
	}
	else if (code == TU)
	{
		tmp[0] = 0.0f;
		tmp[1] = 0.0f;
		tmp[2] = 1.0f;
		vmatmul(fdf->camera.basis, tmp, fdf->camera.orientation);
		add_vectors(fdf->camera.origin, scale_vector(fdf->camera.orientation, 0.1f,  fdf->camera.orientation, 3), fdf->camera.origin, 3);
		look_at(&fdf->camera, fdf);
	}
	else if (code == TD)
	{
		tmp[0] = 0.0f;
		tmp[1] = 0.0f;
		tmp[2] = -1.0f;
		vmatmul(fdf->camera.basis, tmp, fdf->camera.orientation);
		add_vectors(fdf->camera.origin, scale_vector(fdf->camera.orientation, 0.1f,  fdf->camera.orientation, 3), fdf->camera.origin, 3);
		look_at(&fdf->camera, fdf);
	}
	return (0);
}

int	key_release_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
	return (0);
}
