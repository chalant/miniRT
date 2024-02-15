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
	t_ray		ray;
	t_object	*object;
	t_object	*closest;

	if (x < 0 || y < 0 || x > minirt->display.width || y > minirt->display.height)
		return (0);
	closest = NULL;
	to_screen_space(&minirt->display, coords, x, y);
	to_world_space(minirt, coords, position);
	copy_vector(minirt->camera.origin, ray.origin, 3);
	vmatmul(&minirt->camera.inverse_view, position, ray.direction);
	ray.closest_t = FLT_MAX;
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
		add_vectors(ray.origin, scale_vector(ray.direction, ray.closest_t, minirt->mouse.point, 3),
				minirt->mouse.point, 3);
		copy_vector(position, minirt->mouse.world_position, 3);
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
	matmul(&object->basis, axis, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &object->basis, 3);
	return (0);
}

int	rotate_x(t_minirt *minirt, t_matrix *matrix, float dir[3])
{
	float	tmp[4];
	float	cos_a;
	float	sin_a;

	cos_a = cosf(to_rad(minirt->mouse.hit_point * dir[1]));
	sin_a = sinf(to_rad(minirt->mouse.hit_point * dir[1]));
	matrix->points[0][0] = 1.0f;
	matrix->points[1][1] = cos_a;
	matrix->points[2][1] = sin_a;
	matrix->points[1][2] = -sin_a;
	matrix->points[2][2] = cos_a;
	rotate_object(minirt, minirt->selected_object, matrix);
	vmatmul(matrix, minirt->selected_object->orientation, tmp);
	copy_vector(tmp, minirt->selected_object->orientation, 3);
	matrix->points[0][0] = 0.0f;
	matrix->points[1][1] = 0.0f;
	matrix->points[2][1] = 0.0f;
	matrix->points[1][2] = 0.0f;
	matrix->points[2][2] = 0.0f;
	return (1);
}

int	rotate_y(t_minirt *minirt, t_matrix *matrix, float dir[3])
{
	float	tmp[4];
	float	cos_a;
	float	sin_a;

	cos_a = cosf(to_rad(minirt->mouse.hit_point * dir[0]));
	sin_a = sinf(to_rad(minirt->mouse.hit_point * dir[0]));
	matrix->points[0][0] = cos_a;
	matrix->points[0][2] = -sin_a;
	matrix->points[1][1] = 1.0f;
	matrix->points[2][0] = sin_a;
	matrix->points[2][2] = cos_a;
	rotate_object(minirt, minirt->selected_object, matrix);
	vmatmul(matrix, minirt->selected_object->orientation, tmp);
	copy_vector(tmp, minirt->selected_object->orientation, 3);
	matrix->points[0][0] = 0.0f;
	matrix->points[0][2] = 0.0f;
	matrix->points[1][1] = 0.0f;
	matrix->points[2][0] = 0.0f;
	matrix->points[2][2] = 0.0f;
	return (1);
}

int	rotate_z(t_minirt *minirt, t_matrix *matrix, float dir[3])
{
	float	tmp[4];
	float	cos_a;
	float	sin_a;

	cos_a = cosf(to_rad(minirt->mouse.hit_point * dir[2]));
	sin_a = sinf(to_rad(minirt->mouse.hit_point * dir[2]));
	matrix->points[0][0] = cos_a;
	matrix->points[0][2] = -sin_a;
	matrix->points[1][1] = 1.0f;
	matrix->points[2][0] = sin_a;
	matrix->points[2][2] = cos_a;
	rotate_object(minirt, minirt->selected_object, matrix);
	vmatmul(matrix, minirt->selected_object->orientation, tmp);
	copy_vector(tmp, minirt->selected_object->orientation, 3);
	matrix->points[0][0] = 0.0f;
	matrix->points[0][2] = 0.0f;
	matrix->points[1][1] = 0.0f;
	matrix->points[2][0] = 0.0f;
	matrix->points[2][2] = 0.0f;
	return (1);
}

void	apply_rotation(t_minirt *minirt, float dir[4])
{
	rotate_x(minirt, &minirt->mouse.direction, dir);
	rotate_y(minirt, &minirt->mouse.direction, dir);
	rotate_z(minirt, &minirt->mouse.direction, dir);
}

void	apply_scaling(t_minirt *minirt, float dir[4])
{
	//add_vectors(minirt->selected_object->size, dir, minirt->selected_object->size, 3);
	minirt->selected_object->size[0] += -minirt->selected_object->size[0] * dir[0];
	minirt->selected_object->size[1] += minirt->selected_object->size[1] * dir[1];
	minirt->selected_object->size[2] += dir[2];
}

void	apply_translation(t_minirt *minirt, float dir[4], float speed)
{
	minirt->selected_object->center[0] += speed * dir[0];
	minirt->selected_object->center[1] += speed * dir[1];
	minirt->selected_object->center[2] += speed * dir[2];
}

int	mouse_update(int x, int y, t_minirt *minirt)
{
	float	direction[4];
	float	dir[4];
	float	coords[4];
	float	position[4];

	if (!minirt->selected_object)
		return (0);
	to_screen_space(&minirt->display, coords, x, y);
	to_world_space(minirt, coords, position);
	subtract_vectors(position, minirt->mouse.world_position, direction, 3);
	direction[3] = 1.0f;
	vmatmul(&minirt->camera.basis, direction, dir);
	if (minirt->ctrl.pressed)
		apply_rotation(minirt, direction);
	else if (minirt->shift.pressed)
		apply_scaling(minirt, direction);
	else
		apply_translation(minirt, direction, minirt->mouse.hit_point);
	copy_vector(position, minirt->mouse.world_position, 3);
	return (0);
}
