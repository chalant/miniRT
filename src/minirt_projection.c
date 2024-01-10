#include "minirt.h"

int inv_perspective_projector(t_matrix **matrix, t_camera *camera)
{
	homogeneous_matrix(matrix, 4, 4);
	(*matrix)->points[0][0] = (camera->right - camera->left) / (2 * camera->near);
	(*matrix)->points[0][3] = (camera->right + camera->left) / (2 * camera->near);
	(*matrix)->points[1][1] = (camera->top - camera->bottom) / (2 * camera->near);
	(*matrix)->points[1][3] = (camera->top + camera->bottom) / (2 * camera->near);
	(*matrix)->points[2][3] = -1.0f;
	(*matrix)->points[3][2] = (camera->far - camera->near) / (2 * camera->far * camera->near);
	(*matrix)->points[3][3] = (camera->far + camera->near) / (2 * camera->far * camera->near);
	return (0);
}

int	perspective_projector(t_matrix **matrix, t_camera *camera)
{
	homogeneous_matrix(matrix, 4, 4);
	(*matrix)->points[0][0] = (2 * camera->near) / (camera->right - camera->left);
	(*matrix)->points[0][2] = (camera->right + camera->left) / (camera->right - camera->left);
	(*matrix)->points[1][1] = (2 * camera->near) / (camera->top - camera->bottom);
	(*matrix)->points[1][2] = (camera->top + camera->bottom) / (camera->top - camera->bottom);
	(*matrix)->points[2][2] = -(camera->far + camera->near) / (camera->far - camera->near);
	(*matrix)->points[2][3] = -(2 * camera->far * camera->near) / (camera->far - camera->near);
	(*matrix)->points[3][2] = -1;
	return (0);
}
