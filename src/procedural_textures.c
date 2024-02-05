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

float	*bump_perturbation(t_object *object, float uv_coords[2], float bump[3])
{
	float		**bump_map;

	bump_map = object->material->bump_map.points;

	float epsilon = 0.001f;  // Small offset for finite difference
	int u_e = abs((int)((uv_coords[0] + epsilon) * (object->material->bump_map.cols - 1)) % object->material->bump_map.cols);
	int v_e = abs((int)((uv_coords[1] + epsilon) * (object->material->bump_map.rows - 1)) % object->material->bump_map.rows);
	int u = abs((int)(uv_coords[0] * (object->material->bump_map.cols - 1)) % object->material->bump_map.cols);
	int	v = abs((int)(uv_coords[1] * (object->material->bump_map.rows - 1)) % object->material->bump_map.rows);
	int	u_ne = abs((int)((uv_coords[0]  - epsilon) * (object->material->bump_map.cols - 1)) % object->material->bump_map.cols);
	int	v_ne = abs((int)((uv_coords[1] - epsilon) * (object->material->bump_map.rows - 1)) % object->material->bump_map.rows);

	//fprintf(stderr, "COORDS %d %d %d %d %d %d\n", u_e, v_e , u, v, u_ne, v_ne);

	float du;
    // u_ne = fmaxf(u_ne, 0);
	// v_ne = fmaxf(v_ne, 0);
	du = (bump_map[v][u_e] - bump_map[v][u_ne]) / (2.0f * epsilon);
	float dv;
    dv = (bump_map[v_e][u] - bump_map[v_ne][u]) / (2.0f * epsilon);

    // Use the model-view matrix as the normal matrix
    float gradient_object_space[3];
    
    gradient_object_space[0] = object->center[0] + du;
    gradient_object_space[1] = object->center[1] - dv;
    gradient_object_space[2] = object->center[2];

    float bump_strength = 0.00001f;  // Adjust this value based on your preference

    for (int i = 0; i < 3; i++) {
        bump[i] = bump_strength * gradient_object_space[i];
    }
	return (bump);
}
