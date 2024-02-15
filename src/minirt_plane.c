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
	float		point[4];
	float		projected[4];
	float		u[3];
	float		v[3];

	create_basis(object->orientation, u, v);
	subtract_vectors(hit->point, object->center, point, 3);
	projected[0] = dot_product(point, u, 3);
	projected[1] = dot_product(point, v, 3);
	projected[2] = dot_product(point, object->orientation, 3);
	uv_coords[0] = -projected[0];
	uv_coords[1] = projected[1];
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
	copy_vector(object->orientation, hit->normal, 3);
	return (hit->normal);
}

int	create_plane(t_object *object, float normal[4])
{
	copy_vector(normal, object->orientation, 3);
	normalize_vector(object->orientation, object->orientation, 3);
	object->intersect = hit_plane;
	object->normal = plane_normal;
	object->uv_coords = plane_uv_coords;
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	set_basis(&object->basis, object->orientation);
	return (0);
}
