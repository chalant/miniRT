#include "minirt.h"

int	grab_object(t_minirt *minirt, int x, int y)
{
	int			i;
	float		coords[4];
	float		position[4];
	t_object	*object;
	t_ray		ray;

	if (x < 0 || y < 0 || x > minirt->display.width || y > minirt->display.height)
		return (0);
	to_screen_space(&minirt->display, coords, x, y);
	to_world_space(minirt, coords, position);
	ray.origin[0] = minirt->camera.origin[0];
	ray.origin[1] = minirt->camera.origin[1];
	ray.origin[2] = minirt->camera.origin[2];
	vmatmul(&minirt->camera.inverse_view, position, ray.direction);
	ray.closest_t = FLT_MAX;
	t_object	*closest = NULL;
	i = -1;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		if (!object->intersect(object, &ray))
			continue;
		closest = object;
		ray.closest_t = ray.t;
		//todo: get hit point and hit normal for rotations and scalings
		// the normal will be used to determine which side to scale.
		// the hit point will be used to determine the rotation direction. (we use the object basis)
	}
	if (ray.closest_t < FLT_MAX)
	{
		minirt->selected_object = closest;
		minirt->mouse.world_position[0] = position[0];
		minirt->mouse.world_position[1] = position[1];
		minirt->mouse.world_position[2] = position[2];
		minirt->mouse.world_position[3] = position[3];
	}
	return (0);
}

int	scroll_hook(int button, int x, int y, t_minirt *minirt)
{
	(void)x;
	(void)y;

	printf("scroll %d\n", button);
	if (!minirt->selected_object)
		return (0);
	if (button == 4)
		minirt->selected_object->size[0] *= 0.1f;
	else if (button == 5)
		minirt->selected_object->size[0] *= 0.1f;
	return (0);
}

int	mouse_click_hook(int button, int x, int y, t_minirt *minirt)
{

	if (button == 2)
		minirt->mouse.right_click = 1;
	if (button == 1)
	{
		grab_object(minirt, x, y);
		minirt->mouse.x = x;
		minirt->mouse.y = y;
		minirt->mouse.left_click = 1;
	}
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
	{
		minirt->selected_object = NULL;
		minirt->mouse.left_click = 0;
	}
	return (0);
}

int	rotate_object(t_minirt *minirt, t_object *object, t_matrix *axis)
{
	float	origin[4];

	matmul(&object->basis, axis, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &object->basis, 3);
	vmatmul(&object->basis, object->center, origin);
	return (0);
}

//todo: hard code the rotations.
int	mouse_update(int x, int y, t_minirt *minirt)
{
	float	cos_a;
	float	sin_a;
	float	dx;
	float	dy;
	float	dz;
	float	direction[4];
	float	dir[4];
	float	coords[4];
	float	position[4];
	float	tmp[4];

	// if (x < 0 || y < 0 || x > minirt->display.width || y > minirt->display.height)
	// 	return (0);
	if (!minirt->selected_object)
		return (0);
	to_screen_space(&minirt->display, coords, x, y);
	to_world_space(minirt, coords, position);
	// if (!minirt->mouse.capture)
	// 	return (0);
	// if (!minirt->mouse.left_click)
	// 	return (0);
	dx = (position[0] - minirt->mouse.world_position[0]);
	dy = (position[1] - minirt->mouse.world_position[1]);
	dz = (position[2] - minirt->mouse.world_position[2]);
	// if (dy == 0.0f && dx == 0.0f)
	// 	return (0);
	// if (dy != 0.0f)
	// 	dy /= fabsf(dy);
	// if (dx != 0.0f)
	// 	dx /= fabsf(dx);
	// dx *= 5.0f;
	// dy *= 5.0f;
	// dz *= 5.0f;
	direction[0] = dx;
	direction[1] = dy;
	direction[2] = dz;
	direction[3] = 1.0f;
	vmatmul(&minirt->camera.basis, direction, dir);
	// if (minirt->ctrl.pressed)
	// {
		//todo: this should be object specific
		//minirt->selected_object->size[0] += dir[1];
		cos_a = cosf(dir[1]);
	sin_a = sinf(dir[1]);
	
	minirt->mouse.direction.points[0][0] = 1.0f;
	minirt->mouse.direction.points[1][1] = cos_a;
	minirt->mouse.direction.points[2][1] = sin_a;
	minirt->mouse.direction.points[1][2] = -sin_a;
	minirt->mouse.direction.points[2][2] = cos_a;
	matmul(&minirt->selected_object->basis, &minirt->mouse.direction, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &minirt->selected_object->basis, 3);
	
	vmatmul(&minirt->mouse.direction, minirt->selected_object->orientation, tmp);
	minirt->selected_object->orientation[0] = tmp[0];
	minirt->selected_object->orientation[1] = tmp[1];
	minirt->selected_object->orientation[2] = tmp[2];
	cos_a = cosf(dir[0]);
	sin_a = sinf(dir[0]);
	//todo also multiply by the camera basis.
	minirt->mouse.direction.points[0][0] = 0.0f;
	minirt->mouse.direction.points[1][1] = 0.0f;
	minirt->mouse.direction.points[2][1] = 0.0f;
	minirt->mouse.direction.points[1][2] = 0.0f;
	minirt->mouse.direction.points[2][2] = 0.0f;
	
	minirt->mouse.direction.points[0][0] = cos_a;
	minirt->mouse.direction.points[0][2] = -sin_a;
	minirt->mouse.direction.points[1][1] = 1.0f;
	minirt->mouse.direction.points[2][0] = sin_a;
	minirt->mouse.direction.points[2][2] = cos_a;
	matmul(&minirt->selected_object->basis, &minirt->mouse.direction, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &minirt->selected_object->basis, 3);
	
	vmatmul(&minirt->mouse.direction, minirt->selected_object->orientation, tmp);
	minirt->selected_object->orientation[0] = tmp[0];
	minirt->selected_object->orientation[1] = tmp[1];
	minirt->selected_object->orientation[2] = tmp[2];
	
	minirt->mouse.direction.points[0][0] = 0.0f;
	minirt->mouse.direction.points[0][2] = 0.0f;
	minirt->mouse.direction.points[1][1] = 0.0f;
	minirt->mouse.direction.points[2][0] = 0.0f;
	minirt->mouse.direction.points[2][2] = 0.0f;
	// }
	// else
	// {
	// 	minirt->selected_object->center[0] += 5.0f * dir[0];
	// 	minirt->selected_object->center[1] += 5.0f * dir[1];
	// 	minirt->selected_object->center[2] += 5.0f * dir[2];
	// }
	cos_a = cosf(-dir[0]);
	sin_a = sinf(-dir[0]);
	minirt->mouse.direction.points[0][0] = cos_a;
	minirt->mouse.direction.points[0][1] = -sin_a;
	minirt->mouse.direction.points[1][0] = sin_a;
	minirt->mouse.direction.points[1][1] = cos_a;
	minirt->mouse.direction.points[2][2] = 1.0f;
	matmul(&minirt->selected_object->basis, &minirt->mouse.direction, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &minirt->selected_object->basis, 3);
	//matmul(&minirt->camera.basis, &minirt->selected_object->basis, &minirt->tmp, 3);

	vmatmul(&minirt->mouse.direction, minirt->selected_object->orientation, tmp);
	minirt->selected_object->orientation[0] = tmp[0];
	minirt->selected_object->orientation[1] = tmp[1];
	minirt->selected_object->orientation[2] = tmp[2];	

	minirt->mouse.world_position[0] = position[0];
	minirt->mouse.world_position[1] = position[1];
	minirt->mouse.world_position[2] = position[2];
	minirt->mouse.world_position[3] = position[3];

	minirt->mouse.direction.points[0][0] = 0.0f;
	minirt->mouse.direction.points[0][1] = 0.0f;
	minirt->mouse.direction.points[1][0] = 0.0f;
	minirt->mouse.direction.points[1][1] = 0.0f;
	minirt->mouse.direction.points[2][2] = 0.0f;

	//mlx_mouse_move(minirt->window, minirt->display->width / 2, minirt->display->height / 2);
	return (0);
}
