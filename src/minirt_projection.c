#include "minirt.h"

int	perspective_projector(t_matrix **matrix, t_camera *camera)
{
	float		t;

	t = 1 / tanf(to_rad(camera->fov) / 2.0f);
	homogeneous_matrix(matrix, 3, 3);
	(*matrix)->points[0][0] = (700.0f/ 800.0f) * t;
	(*matrix)->points[1][1] = t;
	(*matrix)->points[2][2] = camera->far / (camera->far - camera->near);
	(*matrix)->points[2][3] = (camera->far * camera->near) / (camera->far - camera->near);
	(*matrix)->points[3][2] = -1.0f;
	(*matrix)->points[3][3] = 0.0f;
	return (0);
}
