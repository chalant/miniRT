#include "minirt.h"

float	*checkerboard(t_object *object, float uv_coords[2], float color[4])
{
	t_material	*material;

	material = object->material;
	if (((int)(roundf(uv_coords[0] * material->repeat_pattern)) % 2 == 0) ^ ((int)(roundf(uv_coords[1] * material->repeat_pattern)) % 2 == 0))
	{
		color[0] = object->color[0];
		color[1] = object->color[1];
		color[2] = object->color[2];
		color[3] = object->color[3];
	}
	else
	{
		color[0] = material->color[0];
		color[1] = material->color[1];
		color[2] = material->color[2];
		color[3] = material->color[3];
	}
	return (color);
}
