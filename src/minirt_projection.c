#include "minirt.h"

int	perspective_projector(t_matrix **matrix, t_display *display, t_camera *camera)
{
	float		t;

	t = fabsf(1 / tanf(to_rad(camera->fov / 2.0f)));
	homogeneous_matrix(matrix, 3, 3);
	//todo: malloc errors
	(*matrix)->points[0][0] = t / display->aspect_ratio;
	(*matrix)->points[1][1] = t;
	(*matrix)->points[2][2] = camera->far / (camera->far - camera->near);
	(*matrix)->points[2][3] = (camera->far * camera->near) / (camera->far - camera->near);
	(*matrix)->points[3][2] = -1.0f;
	(*matrix)->points[3][3] = 0.0f;
	return (0);
}
