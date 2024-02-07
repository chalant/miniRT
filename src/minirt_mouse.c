#include "minirt.h"

int	mouse_click_hook(int button, int x, int y, t_minirt *minirt)
{
	if (button == 2)
		minirt->mouse.right_click = 1;
	if (button == 1)
		minirt->mouse.left_click = 1;
	minirt->mouse.x = x;
	minirt->mouse.y = y;
	return (0);
}

int mouse_release_hook(int button, int x, int y, t_minirt *minirt)
{
	(void)x;
	(void)y;
	if (button == 2)
		minirt->mouse.right_click = 0;
	else if (button == 1)
		minirt->mouse.left_click = 0;
	return (0);
}

//todo: hard code the rotations.
int	mouse_update(int x, int y, t_minirt *minirt)
{
	// float	cos_a;
	// float	sin_a;
	// float	dx;
	// float	dy;
	float		coords[4];
	float		position[4];
	int			i;
	t_object	*object;
	static int	iterations = 0;
	t_ray		ray;

	i = -1;
	if (x < 0 || y < 0 || x > minirt->display.width || y > minirt->display.height)
		return (0);
	to_screen_space(&minirt->display, coords, x, y);
	to_world_space(minirt, coords, position);
	ray.origin[0] = minirt->camera.origin[0];
	ray.origin[1] = minirt->camera.origin[1];
	ray.origin[2] = minirt->camera.origin[2];
	vmatmul(minirt->camera.inverse_view, position, ray.direction);
	//add_vectors(minirt->camera.origin, ray.direction, coords, 3);
	ray.closest_t = FLT_MAX;
	t_object	*closest = NULL;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		if (!object->intersect(object, &ray))
			continue;
		closest = object;
		ray.closest_t = ray.t;
	}
	if (ray.closest_t < FLT_MAX)
		printf("INSIDE! %s %d\n", closest->name, iterations);
	iterations++;
	//todo: check against all objects.
	// if (!minirt->mouse.capture)
	// 	return (0);
	// if (!minirt->mouse.left_click)
	// 	return (0);
	// dy = (y - minirt->mouse.x);
	// dx = (x - minirt->mouse.y);
	// if (dy == 0.0f && dx == 0.0f)
	// 	return (0);
	// if (dy != 0.0f)
	// 	dy /= fabsf(dy);
	// if (dx != 0.0f)
	// 	dx /= fabsf(dx);
	// dx *= 0.02f;
	// dy *= 0.02f;
	// cos_a = cosf(dy);
	// sin_a = sinf(dy);
	// minirt->mouse.direction->points[0][0] = 1.0f;
	// minirt->mouse.direction->points[1][1] = cos_a;
	// minirt->mouse.direction->points[2][1] = sin_a;
	// minirt->mouse.direction->points[1][2] = -sin_a;
	// minirt->mouse.direction->points[2][2] = cos_a;
	// matmul(minirt->camera.basis, minirt->mouse.direction, minirt->tmp, 3);
	// matrix_copy(minirt->tmp, minirt->camera.basis, 3);
	// if (dx == 0.0f)
	// 	return (0);
	// cos_a = cosf(dx);
	// sin_a = sinf(dx);
	// minirt->mouse.direction->points[0][0] = 0.0f;
	// minirt->mouse.direction->points[1][1] = 0.0f;
	// minirt->mouse.direction->points[2][1] = 0.0f;
	// minirt->mouse.direction->points[1][2] = 0.0f;
	// minirt->mouse.direction->points[2][2] = 0.0f;
	// minirt->mouse.direction->points[0][0] = cos_a;
	// minirt->mouse.direction->points[0][2] = -sin_a;
	// minirt->mouse.direction->points[1][1] = 1.0f;
	// minirt->mouse.direction->points[2][0] = sin_a;
	// minirt->mouse.direction->points[2][2] = cos_a;
	// matmul(minirt->camera.basis, minirt->mouse.direction, minirt->tmp, 3);
	// matrix_copy(minirt->tmp, minirt->camera.basis, 3);
	// look_at(&minirt->camera, minirt);
	// minirt->mouse.direction->points[0][0] = 0.0f;
	// minirt->mouse.direction->points[0][2] = 0.0f;
	// minirt->mouse.direction->points[1][1] = 0.0f;
	// minirt->mouse.direction->points[2][0] = 0.0f;
	// minirt->mouse.direction->points[2][2] = 0.0f;
	// //mlx_mouse_move(minirt->window, minirt->display->width / 2, minirt->display->height / 2);
	return (0);
}
