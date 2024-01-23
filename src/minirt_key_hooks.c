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

void	copy_vector(float *source, float *dest, int n)
{
	while (--n > -1)
		dest[n] = source[n];
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
	float	tmp[3];
	//float	cam[4];

	matmul(fdf->camera->basis, rot, fdf->tmp, 3);
	matrix_copy(fdf->tmp, fdf->camera->basis, 3);
	print_matrix3(fdf->camera->basis);
	vmatmul(fdf->camera->basis, fdf->camera->origin, tmp);
	//copy_vector(tmp, fdf->camera->origin, 3);
	matrix_copy(fdf->camera->basis, fdf->camera->view, 3);
	set_translate(fdf->camera->view, -tmp[0], -tmp[1], -tmp[2]);
	// vmatmul(rot, fdf->camera->orientation, cam);
	// copy_vector(cam, fdf->camera->orientation, 4);
	//inplace_matmul(rot, fdf->camera->view, fdf->tmp, 3);
	invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
	return (0);
}

int	key_press_hook(int code, t_minirt *fdf)
{
	float	tmp[4];
	float	cam[4];

	if (code == RL)
	{
		vmatmul(fdf->rev_rotations->y_axis, fdf->camera->orientation, cam);
		copy_vector(cam, fdf->camera->orientation, 4);
		//look_at(fdf->camera, fdf);
		rotate_around(fdf, fdf->rotations->y_axis);	
	}
	else if (code == RR)
	{
		vmatmul(fdf->rotations->y_axis, fdf->camera->orientation, cam);
		copy_vector(cam, fdf->camera->orientation, 4);
		rotate_around(fdf, fdf->rev_rotations->y_axis);
		//look_at(fdf->camera, fdf);

	}
	else if (code == RU)
	{
		vmatmul(fdf->rotations->x_axis, fdf->camera->orientation, cam);
		copy_vector(cam, fdf->camera->orientation, 4);
		rotate_around(fdf, fdf->rev_rotations->x_axis);
		//look_at(fdf->camera, fdf);
	}
	else if (code == RD)
	{
		vmatmul(fdf->rev_rotations->x_axis, fdf->camera->orientation, cam);
		copy_vector(cam, fdf->camera->orientation, 4);
		//look_at(fdf->camera, fdf);
		rotate_around(fdf, fdf->rotations->x_axis);
	}
	else if (code == TL)
	{
		cam[0] = fdf->camera->origin[0];
		cam[1] = fdf->camera->origin[1];
		cam[2] = fdf->camera->origin[2];
		cam[3] = 1.0f;

		copy_vector(fdf->camera->orientation, cam, 3);
		//add_vectors(fdf->camera->origin, fdf->camera->orientation, cam, 3);
		scale_vector(cam, 0.1f, 3);
		subtract_vectors(fdf->camera->origin, cam, fdf->camera->origin, 3);

		set_translate(fdf->camera->view, -fdf->camera->origin[0], -fdf->camera->origin[1], -fdf->camera->origin[2]);
		// vmatmul(fdf->rev_translations->x_axis, cam, tmp);
		// copy_vector(tmp, fdf->camera->origin, 3);
		// vmatmul(fdf->camera->basis, fdf->camera->origin, tmp);
		// matrix_copy(fdf->camera->basis, fdf->camera->view, 3);
		// set_translate(fdf->camera->view, -tmp[0], -tmp[1], -tmp[2]);
		// //inplace_matmul(rot, fdf->camera->view, fdf->tmp, 3);
		invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
		//rotate_around(fdf, fdf->rotations->z_axis);	
		// vmatmul(fdf->translations->x_axis, fdf->camera->origin, tmp);
		// copy_vector(tmp, fdf->camera->origin, 3);
		// //matrix_copy(fdf->tmp, fdf->camera->view, 4);
		// invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
	}
	else if (code == TR)
	{
		//todo: use the right vector
		cam[0] = fdf->camera->origin[0];
		cam[1] = fdf->camera->origin[1];
		cam[2] = fdf->camera->origin[2];
		cam[3] = 1.0f;

		copy_vector(fdf->camera->orientation, cam, 3);
		//add_vectors(fdf->camera->origin, fdf->camera->orientation, cam, 3);
		scale_vector(cam, 0.1f, 3);
		add_vectors(fdf->camera->origin, cam, fdf->camera->origin, 3);
		// vmatmul(fdf->translations->x_axis, cam, tmp);
		// copy_vector(tmp, fdf->camera->origin, 3);
		// vmatmul(fdf->camera->basis, fdf->camera->origin, tmp);
		// matrix_copy(fdf->camera->basis, fdf->camera->view, 3);
		set_translate(fdf->camera->view, -fdf->camera->origin[0], -fdf->camera->origin[1], -fdf->camera->origin[2]);
		// //inplace_matmul(rot, fdf->camera->view, fdf->tmp, 3);
		invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
		//rotate_around(fdf, fdf->rev_rotations->z_axis);	
		// matmul(fdf->camera->view, fdf->rev_translations->x_axis, fdf->tmp, 4);
		// matrix_copy(fdf->tmp, fdf->camera->view, 4);
		// invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
	}
	else if (code == TU)
	{
		cam[0] = fdf->camera->origin[0];
		cam[1] = fdf->camera->origin[1];
		cam[2] = fdf->camera->origin[2];
		cam[3] = 1.0f;
	
		copy_vector(fdf->camera->orientation, cam, 3);
		//add_vectors(fdf->camera->origin, fdf->camera->orientation, cam, 3);
		scale_vector(cam, 0.1f, 3);
		add_vectors(fdf->camera->origin, cam, fdf->camera->origin, 3);
		fprintf(stderr, "cam %f %f %f\n", cam[0], cam[1], cam[2]);
		vmatmul(fdf->camera->basis, fdf->camera->origin, tmp);
		set_translate(fdf->camera->view, -tmp[0], -tmp[1], -tmp[2]);
		// vmatmul(fdf->rev_translations->z_axis, fdf->camera->orientation, tmp);
		// fprintf(stderr, "tmp %f %f %f\n", tmp[0], tmp[1], tmp[2]);
		// add_vectors(fdf->camera->origin, tmp, fdf->camera->origin, 3);
		// //copy_vector(tmp, fdf->camera->origin, 3);
		// vmatmul(fdf->camera->basis, fdf->camera->origin, tmp);
		// matrix_copy(fdf->camera->basis, fdf->camera->view, 3);
		//set_translate(fdf->camera->view, -fdf->camera->origin[0], -fdf->camera->origin[1], -fdf->camera->origin[2]);
		//inplace_matmul(rot, fdf->camera->view, fdf->tmp, 3);
		invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
		// matmul(fdf->camera->view, fdf->translations->z_axis, fdf->tmp, 4);
		// matrix_copy(fdf->tmp, fdf->camera->view, 4);
		//invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
	}
	else if (code == TD)
	{
		cam[0] = fdf->camera->origin[0];
		cam[1] = fdf->camera->origin[1];
		cam[2] = fdf->camera->origin[2];
		cam[3] = 1.0f;
		copy_vector(fdf->camera->orientation, cam, 3);
		//add_vectors(fdf->camera->origin, fdf->camera->orientation, cam, 3);
		scale_vector(cam, 0.1f, 3);
		subtract_vectors(fdf->camera->origin, cam, fdf->camera->origin, 3);
		vmatmul(fdf->camera->basis, fdf->camera->origin, tmp);
		set_translate(fdf->camera->view, -tmp[0], -tmp[1], -tmp[2]);
		//set_translate(fdf->camera->view, -fdf->camera->origin[0], -fdf->camera->origin[1], -fdf->camera->origin[2]);
		//inplace_matmul(rot, fdf->camera->view, fdf->tmp, 3);
		invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
		// matmul(fdf->camera->view, fdf->rev_translations->z_axis, fdf->tmp, 4);
		// matrix_copy(fdf->tmp, fdf->camera->view, 4);
		// invert_matrix(fdf->camera->view, fdf->camera->inverse_view, fdf->tmp, 4);
	}
	return (0);
}

int	key_release_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
	return (0);
}
