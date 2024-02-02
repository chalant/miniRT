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
	fprintf(stderr, "COORDS %d %d %f %f\n",(int)((uv_coords[0] - epsilon)), (int)((uv_coords[1] - epsilon) * (object->material->bump_map.rows - 1)), uv_coords[0], uv_coords[1]);
    float du = (bump_map[(int)(uv_coords[1] * (object->material->bump_map.rows - 1))][(int)((uv_coords[0] + epsilon) * (object->material->bump_map.cols - 1))] - bump_map[(int)(uv_coords[1] * (object->material->bump_map.rows - 1))][(int)((uv_coords[0] - epsilon) * (object->material->bump_map.cols - 1))]) / (2.0f * epsilon);
    float dv = (bump_map[(int)((uv_coords[1] + epsilon) * (object->material->bump_map.rows - 1))][(int)(uv_coords[0] * (object->material->bump_map.cols - 1))] - bump_map[(int)((uv_coords[1] - epsilon) * (object->material->bump_map.rows - 1))][(int)(uv_coords[0] * (object->material->bump_map.cols - 1))]) / (2.0f * epsilon);

    // Use the model-view matrix as the normal matrix
    float gradient_object_space[3];
    
    gradient_object_space[0] = du + dv;
    gradient_object_space[1] = du + dv;
    gradient_object_space[2] = du + dv;

    float bump_strength = 0.1f;  // Adjust this value based on your preference

    for (int i = 0; i < 3; i++) {
        bump[i] = bump_strength * gradient_object_space[i];
    }

    // // Assuming bump_value represents displacement, adjust the perturbation strength as needed
    // float bump_strength = 0.1;  // Adjust this value based on your preference

    // // Perturb the normal based on the bump map
    // for (int i = 0; i < 3; i++) {
    //     bump[i] = bump_strength * bump_value / 255.0f;
    // }

	// bump[0] = du;
	// bump[1] = dv;
	// bump[2] = 1.0f;
	normalize_vector(bump, bump, 3);
	return (bump);
}
