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
	// float		du = 0.0f;
	// float		dv = 0.0f;

	bump_map = object->material->bump_map.points;
	// if (uv_coords[0] + 1 <= object->material->bump_map.rows && uv_coords[0] - 1 >= 0)
	// 	du = bump_map[(int)uv_coords[0] + 1][(int)uv_coords[1]] - bump_map[(int)uv_coords[0] - 1][(int)uv_coords[1]];
	// if (uv_coords[1] + 1 <= object->material->bump_map.cols && uv_coords[1] - 1 >= 0)
	// 	dv = bump_map[(int)uv_coords[0]][(int)uv_coords[1] + 1] - bump_map[(int)uv_coords[0]][(int)uv_coords[1] - 1];
	
	// int u = (int)(uv_coords[0] * (object->material->bump_map.cols - 1));
    // int v = (int)(uv_coords[1] * (object->material->bump_map.rows - 1));
	//du = bump_map[u + 1][(int)uv_coords[1]] - bump_map[(int)uv_coords[0] - 1][(int)uv_coords[1]];
	// float bump_value = bump_map[v][u];

	float epsilon = 0.001f;  // Small offset for finite difference
	int u_e = (int)((uv_coords[0] + epsilon) * (object->material->bump_map.cols - 1)) % 1024;
	int v_e = (int)((uv_coords[1] + epsilon) * (object->material->bump_map.rows - 1)) % 1024;
	int u = (int)(uv_coords[0] * (object->material->bump_map.cols - 1));
	int	v = (int)(uv_coords[1] * (object->material->bump_map.rows - 1));
	int	u_ne = (int)((uv_coords[0] - epsilon) * (object->material->bump_map.cols - 1));
	int	v_ne = (int)((uv_coords[1] - epsilon) * (object->material->bump_map.rows - 1));

	//fprintf(stderr, "COORDS %d %d %d %d %d %d\n", u_e, v_e , u, v, (int)((uv_coords[0] - epsilon) * (object->material->bump_map.cols - 1)), (int)((uv_coords[1] - epsilon) * (object->material->bump_map.rows - 1)));

	float du;
    if (u_ne < 0)
		u_ne = u_ne - u_ne;
	du = (bump_map[v][u_e] - bump_map[v][u_ne]) / (2.0f * epsilon);
	float dv;
    dv = (bump_map[v_e][u] - bump_map[v_ne][u]) / (2.0f * epsilon);

    // Use the model-view matrix as the normal matrix
    float gradient_object_space[3];
    
    gradient_object_space[0] = object->center[0] * du + object->center[0] * dv;
    gradient_object_space[1] = object->center[1] * du + object->center[1] * dv;
    gradient_object_space[2] = object->center[2] * du +  object->center[2] * dv;

    float bump_strength = 0.00005f;  // Adjust this value based on your preference

    for (int i = 0; i < 3; i++) {
        bump[i] = bump_strength * gradient_object_space[i];
    }
	//fprintf(stderr, "BUMP %f %f %f\n", bump[0], bump[1], bump[2]);
    // // Assuming bump_value represents displacement, adjust the perturbation strength as needed
    // float bump_strength = 0.1;  // Adjust this value based on your preference

    // // Perturb the normal based on the bump map
    // for (int i = 0; i < 3; i++) {
    //     bump[i] = bump_strength * bump_value / 255.0f;
    // }

	// bump[0] = du;
	// bump[1] = dv;
	// bump[2] = 1.0f;
	//normalize_vector(bump, bump, 3);
	return (bump);
}
