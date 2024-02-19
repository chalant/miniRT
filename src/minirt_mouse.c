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

int	mouse_hit(t_minirt *minirt, t_ray *ray, t_object **closest)
{
	int			i;
	t_object	*object;

	*closest = NULL;
	ray->closest_t = FLT_MAX;
	i = -1;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		if (!object->intersect(object, ray))
			continue ;
		*closest = object;
		ray->closest_t = ray->t;
	}
	return (ray->closest_t < FLT_MAX);
}

int	grab_object(t_minirt *minirt, int x, int y)
{
	float		position[4];
	t_ray		ray;
	t_object	*closest;

	if (x < 0 || y < 0 || x > minirt->display.width
		|| y > minirt->display.height)
		return (0);
	to_screen_space(&minirt->display, ray.direction, x, y);
	to_world_space(minirt, ray.direction, position);
	copy_vector(minirt->camera.position, ray.origin, 3);
	matvec_product(&minirt->camera.inverse_view, position, ray.direction);
	if (!mouse_hit(minirt, &ray, &closest))
		return (0);
	minirt->selected_object = closest;
	minirt->mouse.hit_point = ray.closest_t;
	copy_vector(position, minirt->mouse.world_position, 3);
	return (1);
}

int	mouse_click_hook(int button, int x, int y, t_minirt *minirt)
{
	if (button == 1)
	{
		grab_object(minirt, x, y);
		minirt->mouse.left_click = 1;
	}
	minirt->mouse.x = x;
	minirt->mouse.y = y;
	return (0);
}

int	mouse_release_hook(int button, int x, int y, t_minirt *minirt)
{
	(void)x;
	(void)y;
	if (!minirt->key_pressed)
		minirt->render_mode = full_resolution;
	if (button == 1)
	{
		minirt->selected_object = NULL;
		minirt->mouse.left_click = 0;
	}
	return (0);
}

int	rotate_object(t_minirt *minirt, t_object *object, t_matrix *axis)
{
	matrix_product(&object->basis, axis, &minirt->tmp, 3);
	matrix_copy(&minirt->tmp, &object->basis, 3);
	return (0);
}

int	rotate_x(t_minirt *minirt, t_matrix *matrix, float dir[3])
{
	float	tmp[4];

	x_rotation(matrix, 5.0f * to_rad(minirt->mouse.hit_point * dir[1]));
	rotate_object(minirt, minirt->selected_object, matrix);
	matvec_product(matrix, minirt->selected_object->orientation, tmp);
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

	y_rotation(matrix, 5.0f * to_rad(minirt->mouse.hit_point * dir[0]));
	rotate_object(minirt, minirt->selected_object, matrix);
	matvec_product(matrix, minirt->selected_object->orientation, tmp);
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

	z_rotation(matrix, minirt->mouse.hit_point * dir[0]);
	rotate_object(minirt, minirt->selected_object, matrix);
	matvec_product(matrix, minirt->selected_object->orientation, tmp);
	copy_vector(tmp, minirt->selected_object->orientation, 3);
	matrix->points[0][0] = 0.0f;
	matrix->points[0][1] = 0.0f;
	matrix->points[1][0] = 0.0f;
	matrix->points[1][1] = 0.0f;
	matrix->points[2][2] = 0.0f;
	return (1);
}

void	apply_rotation(t_minirt *minirt, float dir[4])
{
	minirt->render_mode = minirt->low_res;
	rotate_x(minirt, &minirt->mouse.direction, dir);
	rotate_y(minirt, &minirt->mouse.direction, dir);
	if (minirt->shift.pressed)
		rotate_z(minirt, &minirt->mouse.direction, dir);
}

void	apply_scaling(t_minirt *minirt, float dir[4])
{
	t_object	*selected;

	minirt->render_mode = minirt->low_res;
	selected = minirt->selected_object;
	minirt->selected_object->size[0] += -selected->size[0] * dir[0];
	minirt->selected_object->size[1] += selected->size[1] * dir[1];
	minirt->selected_object->size[2] += dir[2];
}

void	apply_translation(t_minirt *minirt, float dir[4], float speed)
{
	minirt->render_mode = minirt->low_res;
	minirt->selected_object->center[0] += speed * dir[0];
	minirt->selected_object->center[1] += speed * dir[1];
	minirt->selected_object->center[2] += speed * dir[2];
}

int	update_pattern(t_material *material, float direction[4])
{
	material->repeat_pattern += direction[1];
	return (0);
}

int	update_material(t_minirt *minirt, t_object *object, float direction[4])
{
	t_material	*material;

	material = ft_darray_get(&minirt->materials, object->material_index);
	if (minirt->ctrl.pressed)
		return (update_pattern(material, direction));
	material->shininess += direction[1] * minirt->mouse.hit_point * 5.0f;
	if (material->shininess < 2.0f)
		material->shininess = 2.0f;
	else if (material->shininess > 200.0f)
		material->shininess = 200.0f;
	material->reflectivity += -direction[0];
	if (material->reflectivity >= 0.5f)
		material->reflectivity = 0.5f;
	else if (material->reflectivity < 0.0f)
		material->reflectivity = 0.0f;
	return (0);
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
	matvec_product(&minirt->camera.basis, direction, dir);
	if (minirt->alt.pressed)
		update_material(minirt, minirt->selected_object, dir);
	else if (minirt->ctrl.pressed)
		apply_rotation(minirt, dir);
	else if (minirt->shift.pressed)
		apply_scaling(minirt, dir);
	else
		apply_translation(minirt, dir, minirt->mouse.hit_point);
	copy_vector(position, minirt->mouse.world_position, 3);
	return (0);
}
