/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:06:20 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:06:21 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	}
	if (ray.closest_t < FLT_MAX)
	{
		minirt->selected_object = closest;
		minirt->mouse.hit_point = ray.closest_t;
		minirt->mouse.world_position[0] = position[0];
		minirt->mouse.world_position[1] = position[1];
		minirt->mouse.world_position[2] = position[2];
		minirt->mouse.world_position[3] = position[3];
	}
	return (0);
}

int	mouse_click_hook(int button, int x, int y, t_minirt *minirt)
{

	if (button == 2)
		minirt->mouse.right_click = 1;
	if (button == 1)
	{
		grab_object(minirt, x, y);
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
	//float	origin[4];

	matmul(&object->basis, axis, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &object->basis, 3);
	//vmatmul(&object->basis, object->center, origin);
	return (0);
}

int	set_x_rotation(t_matrix *matrix, float cos_a, float sin_a)
{
	matrix->points[0][0] = 1.0f;
	matrix->points[1][1] = cos_a;
	matrix->points[2][1] = sin_a;
	matrix->points[1][2] = -sin_a;
	matrix->points[2][2] = cos_a;
	return (1);
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
	dx = (position[0] - minirt->mouse.world_position[0]);
	dy = (position[1] - minirt->mouse.world_position[1]);
	dz = (position[2] - minirt->mouse.world_position[2]);
	direction[0] = dx;
	direction[1] = dy;
	direction[2] = dz;
	direction[3] = 1.0f;
	float	u[3];
	float	v[3];
	vmatmul(&minirt->camera.basis, direction, dir);
	if (minirt->ctrl.pressed)
	{
		cos_a = cosf(minirt->mouse.hit_point * dir[1]);
		sin_a = sinf(minirt->mouse.hit_point * dir[1]);
	
		minirt->mouse.direction.points[0][0] = 1.0f;
		minirt->mouse.direction.points[1][1] = cos_a;
		minirt->mouse.direction.points[2][1] = sin_a;
		minirt->mouse.direction.points[1][2] = -sin_a;
		minirt->mouse.direction.points[2][2] = cos_a;
		// matmul(&minirt->selected_object->basis, &minirt->mouse.direction, &minirt->tmp, 3);
		// matrix_copy(&minirt->tmp, &minirt->selected_object->basis, 3);
		rotate_object(minirt, minirt->selected_object, &minirt->mouse.direction);
		create_basis(minirt->selected_object->orientation, u, v);
	
		// dir[0] = dot_product(dir, u, 3);
		// dir[1] = dot_product(dir, v, 3);
		// dir[2] = dot_product(dir, minirt->selected_object->orientation, 3);
		vmatmul(&minirt->mouse.direction, minirt->selected_object->orientation, tmp);
		minirt->selected_object->orientation[0] = tmp[0];
		minirt->selected_object->orientation[1] = tmp[1];
		minirt->selected_object->orientation[2] = tmp[2];
		cos_a = cosf(minirt->mouse.hit_point * dir[0]);
		sin_a = sinf(minirt->mouse.hit_point * dir[0]);
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
		// matmul(&minirt->selected_object->basis, &minirt->mouse.direction, &minirt->tmp, 3);
		// matrix_copy(&minirt->tmp, &minirt->selected_object->basis, 3);
		rotate_object(minirt, minirt->selected_object, &minirt->mouse.direction);
		
		vmatmul(&minirt->mouse.direction, minirt->selected_object->orientation, tmp);
		minirt->selected_object->orientation[0] = tmp[0];
		minirt->selected_object->orientation[1] = tmp[1];
		minirt->selected_object->orientation[2] = tmp[2];
		
		minirt->mouse.direction.points[0][0] = 0.0f;
		minirt->mouse.direction.points[0][2] = 0.0f;
		minirt->mouse.direction.points[1][1] = 0.0f;
		minirt->mouse.direction.points[2][0] = 0.0f;
		minirt->mouse.direction.points[2][2] = 0.0f;

		cos_a = cosf(-dir[0]);
		sin_a = sinf(-dir[0]);
		minirt->mouse.direction.points[0][0] = cos_a;
		minirt->mouse.direction.points[0][1] = -sin_a;
		minirt->mouse.direction.points[1][0] = sin_a;
		minirt->mouse.direction.points[1][1] = cos_a;
		minirt->mouse.direction.points[2][2] = 1.0f;
		// matmul(&minirt->selected_object->basis, &minirt->mouse.direction, &minirt->tmp, 3);
		// matrix_copy(&minirt->tmp, &minirt->selected_object->basis, 3);
		rotate_object(minirt, minirt->selected_object, &minirt->mouse.direction);
		//matmul(&minirt->camera.basis, &minirt->selected_object->basis, &minirt->tmp, 3);

		vmatmul(&minirt->mouse.direction, minirt->selected_object->orientation, tmp);
		minirt->selected_object->orientation[0] = tmp[0];
		minirt->selected_object->orientation[1] = tmp[1];
		minirt->selected_object->orientation[2] = tmp[2];

		minirt->mouse.direction.points[0][0] = 0.0f;
		minirt->mouse.direction.points[0][1] = 0.0f;
		minirt->mouse.direction.points[1][0] = 0.0f;
		minirt->mouse.direction.points[1][1] = 0.0f;
		minirt->mouse.direction.points[2][2] = 0.0f;
	}
	else if (minirt->shift.pressed)
		add_vectors(minirt->selected_object->size, dir, minirt->selected_object->size, 3);
	else
	{
		minirt->selected_object->center[0] += minirt->mouse.hit_point * dir[0];
		minirt->selected_object->center[1] += minirt->mouse.hit_point * dir[1];
		minirt->selected_object->center[2] += minirt->mouse.hit_point * dir[2];
	}

	minirt->mouse.world_position[0] = position[0];
	minirt->mouse.world_position[1] = position[1];
	minirt->mouse.world_position[2] = position[2];
	minirt->mouse.world_position[3] = position[3];
	//mlx_mouse_move(minirt->window, minirt->display->width / 2, minirt->display->height / 2);
	return (0);
}
