/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_cone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:05:01 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:05:02 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*cone_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
{
	float		theta;
	float		phi;
	float		point[4];
	float		center[4];

	vmatmul(&object->basis, hit->point, point);
	vmatmul(&object->basis, object->center, center);
	phi = atan2f(point[2] - center[2], point[0] - center[0]);
	theta = acosf((point[1] - center[1]) / object->size[0]);
	uv_coords[0] = 0.0f;
	uv_coords[1] = 0.0f;
	return (uv_coords);
}

float	*cone_normal(t_object *object, t_hit *hit)
{
	float	v[3];

	subtract_vectors(hit->point, object->center, v, 3);
	scale_vector(v, dot_product(v, object->orientation, 3) / dot_product(v, v, 3), v, 3);
	subtract_vectors(v, object->orientation, hit->normal, 3);
	normalize_vector(hit->normal, hit->normal, 3);
	return (hit->normal);
}

int	hit_cone(t_object *object, t_ray *ray)
{
	float	abc[3];
	float	ro;
	float	oc;
	float	center[3];
	float	discriminant;
	float	t;

	subtract_vectors(ray->origin, object->center, center, 3);
	ro = dot_product(ray->direction, object->orientation, 3);
	oc = dot_product(object->orientation, center, 3);
	abc[0] = ro - (object->size[0] * object->size[0]);
	abc[1] = 2.0f * (ro * oc - dot_product(ray->direction, center, 3) * (object->size[0] * object->size[0]));
	abc[2] = oc * oc - dot_product(center, center, 3) * object->size[0] * object->size[0];
	discriminant = abc[1] * abc[1] - 4.0f * abc[0] * abc[2];
	if (discriminant < 0.0f)
		return (0);
	t = (-abc[1] - sqrtf(discriminant)) / (2 * abc[0]);
	if (t < 0.0f || t > object->size[1] || t > abc[1])
		t = (-abc[1] + sqrtf(discriminant)) / (2 * abc[0]);
	if (t < 0.0f)
		return (0);
	ray->t = t;
	return (1);
}

int create_cone(t_object *object, float height, float radius)
{
	object->intersect = hit_cone;
	object->normal = cone_normal;
	object->uv_coords = cone_uv_coords;
	object->size[0] = cosf(to_rad(radius));
	object->size[1] = height;
	object->size[2] = 0.0f;
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	set_basis(&object->basis, (float[3]){1.0f, 0.0f, 0.0f});
	return (0);
}
