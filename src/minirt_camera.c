#include "minirt.h"

int	set_vtranslate(t_matrix *matrix, float v[3])
{
	matrix->points[0][matrix->cols - 1] = v[0];
	matrix->points[1][matrix->cols - 1] = v[1];
	matrix->points[2][matrix->cols - 1] = v[2];
	return (0);
}

int	set_col(t_matrix *matrix, float *values, int col, int n)
{
	while (--n > -1)
		matrix->points[n][col] = values[n];
	return (0);
}

int	compute_angles(float result[3], float orientation[3])
{
	float	normal[3];
	float	up[3];

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
	cross_product(orientation, up, normal);
	result[0] = atan2f(normal[0], normal[1]);
	result[1] = atan2f(orientation[0], orientation[2]);
	result[2] = asinf(orientation[1]);
	return (0);
}

float	*choose_vector(float vec1[3], float vec2[3], float vec3[3], float orientation[3])
{
	float	v2;
	float	max_v;
	float	*max;

	max = vec1;
	max_v = fabsf(dot_product(vec1, orientation, 3) - 1.0f);
	v2 = fabsf(dot_product(vec2, orientation, 3) - 1.0f);
	if (max_v < v2)
	{
		max = vec2;
		max_v = v2;
	}
	if (max_v < fabsf(dot_product(vec3, orientation, 3) - 1.0f))
		max = vec3;
	return (max);
}

int print_matrix2(t_matrix *matrix)
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
	return (0);
}

int	look_at(t_camera *camera, t_minirt *fdf)
{
	float		tmp[4];

	vmatmul(camera->basis, camera->origin, tmp);
	invert_matrix(camera->basis, camera->view, fdf->tmp, 3);
	set_translate(camera->view, -camera->origin[0], -camera->origin[1], -camera->origin[2]);
	matrix_copy(camera->basis, camera->inverse_view, 3);
	set_translate(camera->inverse_view, camera->origin[0], camera->origin[1], camera->origin[2]);
	//invert_matrix(camera->view, camera->inverse_view, fdf->tmp, 4);
	return (0);
}

//todo: handle errors.
int	set_camera_transform(t_camera *camera)
{
	t_matrix	*tmp;
	// float		*vec;
	float		up[3];

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;
	if (homogeneous_matrix(&tmp, 3, 3) == -1)
		return (-1);
	set_diagonal(tmp, 1.0f);
	normalize_vector(camera->orientation, camera->orientation, 3);
	// vec = choose_vector(tmp->points[0], tmp->points[1], tmp->points[1], camera->orientation);
	cross_product(up, camera->orientation, camera->right);
	normalize_vector(camera->right, camera->right, 3);
	cross_product(camera->orientation, camera->right, camera->up);
	normalize_vector(camera->up, camera->up, 3);
	if (homogeneous_matrix(&camera->basis, 2, 2) == -1)
		return (-1);
	if (homogeneous_matrix(&camera->inverse_view, 3, 3) == -1)
		return (-1);
	if (homogeneous_matrix(&camera->view, 3, 3) == -1)
		return (-1);
	set_col(camera->basis, camera->right, 0, 3);
	set_col(camera->basis, camera->up, 1, 3);
	set_col(camera->basis, camera->orientation, 2, 3);
	matrix_copy(camera->basis, camera->view, 3);
	set_translate(camera->view, -camera->origin[0], -camera->origin[2], -camera->origin[2]);
	invert_matrix(camera->view, camera->inverse_view, tmp, 4);
	print_matrix2(camera->view);
	print_matrix2(camera->inverse_view);
	delete_matrix(tmp);
	return (0);
}
