#include "minirt.h"

int	set_vtranslate(t_matrix *matrix, float v[3])
{
	matrix->points[0][matrix->cols - 1] = v[0];
	matrix->points[1][matrix->cols - 1] = v[1];
	matrix->points[2][matrix->cols - 1] = v[2];
	return (0);
}

int	set_col(t_matrix *matrix, int col, float value)
{
	matrix->points[0][col] = value;
	matrix->points[1][col] = value;
	matrix->points[2][col] = value;
	return (0);
}

int	cross_product(float v1[3], float v2[3], float result[3])
{
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
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

// int	set_view_matrix(t_minirt *minirt, t_camera *camera)
// {
// 	t_matrix	*rotation;
// 	t_matrix	*tmp;

// 	homogeneous_matrix(&minirt->view_matrix, 3, 3);
// 	minirt->vi
// 	return (0);
// }

//todo: handle errors.
int	set_camera_transform(t_camera *camera, t_display *display)
{
	float		angles[3];
	t_matrix	*tmp;

	//todo: use a cache for this.
	homogeneous_matrix(&tmp, 3, 3);
	(void)display;
	compute_angles(angles, camera->orientation);
	homogeneous_matrix(&camera->inverse_transform, 3, 3);
	set_diagonal(camera->inverse_transform, 1.0f);
	homogeneous_matrix(&camera->transform, 3, 3);
	set_diagonal(camera->transform, 1.0f);
	camera->transform->points[1][1] = 1.0f;
	set_vtranslate(camera->transform, camera->origin);
	//matrix_copy(camera->transform, camera->inverse_transform, tmp, 4);
	invert_matrix(camera->transform, camera->inverse_transform, tmp, 4);
	return (0);
}
