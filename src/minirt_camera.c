#include "minirt.h"

int	set_vtranslate(t_matrix *matrix, float v[3])
{
	matrix->points[0][matrix->cols - 1] = v[0];
	matrix->points[1][matrix->cols - 1] = v[1];
	matrix->points[2][matrix->cols - 1] = v[2];
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
	up[1] = -1;
	up[2] = 0;
	cross_product(orientation, up, normal);
	result[0] = atan2f(normal[0], normal[1]);
	result[1] = atan2f(orientation[0], orientation[2]);
	result[2] = asinf(orientation[1]);
	return (0);
}

//todo: handle errors.
int	set_camera_transform(t_camera *camera, t_display *display)
{
	t_matrix	*rotation;
	t_matrix	*tmp;
	float		angles[3];

	compute_angles(angles, camera->orientation);
	homogeneous_matrix(&camera->inverse_transform, 3, 3);
	homogeneous_matrix(&camera->transform, 3, 3);
	set_diagonal(camera->transform, 1.0f);
	homogeneous_matrix(&rotation, 3, 3);
	homogeneous_matrix(&tmp, 3, 3);
	// set_diagonal(camera->transform, 1.0f);
	// x_rotation(camera->transform, angles[0]);
	// y_rotation(rotation, angles[1]);
	// inplace_matmul(camera->transform, rotation, tmp);
	// z_rotation(rotation, angles[2]);
	// inplace_matmul(tmp, rotation, camera->transform);
	mattranspose(camera->transform, camera->inverse_transform);
	set_vtranslate(camera->transform, camera->origin);
	set_vtranslate(camera->inverse_transform, display->origin);
	delete_matrix(tmp);
	delete_matrix(rotation);
	return (0);
}
