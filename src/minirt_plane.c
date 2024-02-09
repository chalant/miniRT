/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:06:29 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:06:30 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*plane_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
{
	float		*normal;
	float		point[4];
	float		center[4];

	vmatmul(&object->basis, hit->point, point);
	vmatmul(&object->basis, object->center, center);
	normal = object->orientation;
	if (fabsf(normal[0]) > fabsf(normal[1]) && fabsf(normal[0]) > fabsf(normal[2]))
	{
		uv_coords[0] = hit->point[1] - object->center[1];
		uv_coords[1] = hit->point[2] - object->center[2];
	}
	else if (fabsf(normal[1]) > fabsf(normal[0]) && fabsf(normal[1]) > fabsf(normal[2]))
	{
		uv_coords[0] = -hit->point[0] + object->center[0];
		uv_coords[1] = hit->point[2] - object->center[2];
	}
	else
	{
		uv_coords[0] = hit->point[0] - object->center[0];
		uv_coords[1] = hit->point[1] - object->center[1];
	}
	return (uv_coords);
}

int	hit_plane(t_object *object, t_ray *ray)
{
	float		denominator;
	float		numerator;
	float		t;
	float		result[3];

	denominator = -dot_product(object->orientation, ray->direction, 3);
	if (fabsf(denominator) < 0.0001f)
		return (0);
	subtract_vectors(ray->origin, object->center, result, 3);
	numerator = dot_product(object->orientation, result, 3);
	t = (numerator / denominator);
	if (t < 0.0f)
		return (0);
	if (t > ray->closest_t)
		return (0);
	ray->t = t;
	return (1);
}

float	*plane_normal(t_object *object, t_hit *hit)
{
	hit->normal[0] = object->orientation[0];
	hit->normal[1] = object->orientation[1];
	hit->normal[2] = object->orientation[2];
	return (hit->normal);
}

int	create_plane(t_object *object, float normal[4])
{
	float	axis[4];

	object->orientation[0] = normal[0];
	object->orientation[1] = normal[1];
	object->orientation[2] = normal[2];
	normalize_vector(object->orientation, object->orientation, 3);
	object->intersect = hit_plane;
	object->normal = plane_normal;
	object->uv_coords = plane_uv_coords;
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	cross_product((float[3]){0.0f, 0.0f, 1.0f}, object->orientation, axis);
	set_basis(&object->basis, axis);
	return (0);
}
