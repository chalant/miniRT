#include "minirt.h"

float	*checkerboard(t_material *material, float uv_coords[2], float color[4])
{
	//fprintf(stderr, "XOR %f\n", (uv_coords[0] * 10));
	if ((int)(uv_coords[0] * 10) % 2 == 0 ^ (int)(uv_coords[1] * 10) % 2 == 0)
	{
		color[0] = material->dark_color[0];
		color[1] = material->dark_color[1];
		color[2] = material->dark_color[2];
		color[3] = material->dark_color[3];
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
