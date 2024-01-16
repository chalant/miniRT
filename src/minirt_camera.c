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
	if (max_v < fabsf(dot_product(vec2, orientation, 3) - 1.0f))
		max = vec3;
	return (max);
}

//todo: handle errors.
int	set_camera_transform(t_camera *camera, t_display *display)
{
	t_matrix	*tmp;
	float		*vec;
	float		normal[3];

	(void)display;
	homogeneous_matrix(&tmp, 3, 3);
	set_diagonal(tmp, 1.0f);
	normalize_vector(camera->orientation, 3);
	vec = choose_vector(tmp->points[0], tmp->points[1], tmp->points[1], camera->orientation);
	cross_product(vec, camera->orientation, normal);
	homogeneous_matrix(&camera->inverse_transform, 3, 3);
	homogeneous_matrix(&camera->transform, 3, 3);
	set_col(camera->transform, vec, 0, 3);
	set_col(camera->transform, normal, 1, 3);
	set_col(camera->transform, camera->orientation, 2, 3);
	camera->transform->points[1][1] = 1.0f;
	set_vtranslate(camera->transform, camera->origin);
	invert_matrix(camera->transform, camera->inverse_transform, tmp, 4);\
	delete_matrix(tmp);
	return (0);
}
