/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_sphere.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:00:08 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:00:09 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*sphere_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
{
	float		theta;
	float		phi;
	float		point[4];
	float		center[4];

	matvec_product(&object->basis, hit->point, point);
	matvec_product(&object->basis, object->center, center);
	phi = atan2f(point[2] - center[2], point[0] - center[0]);
	theta = acosf((point[1] - center[1]) / object->size[1]);
	uv_coords[0] = (phi + M_PI) / (2.0f * M_PI);
	uv_coords[1] = 1.0f - theta / M_PI;
	return (uv_coords);
}

float	*sphere_normal(t_object *object, t_hit *hit)
{
	float		center[3];

	subtract_vectors(hit->ray_origin, object->center, center, 3);
	add_vectors(hit->point, center, hit->normal, 3);
	scale_vector(hit->normal, 1 / object->size[1], hit->normal, 3);
	return (hit->normal);
}

int	hit_sphere(t_object *object, t_ray *ray)
{
	float		t;
	float		abc[3];
	float		discriminant;
	float		center[3];

	subtract_vectors(ray->origin, object->center, center, 3);
	abc[0] = dot_product(ray->direction, ray->direction, 3);
	abc[1] = 2.0f * dot_product(center, ray->direction, 3);
	abc[2] = dot_product(center, center, 3) - object->size[1] * object->size[1];
	discriminant = abc[1] * abc[1] - 4 * abc[0] * abc[2];
	if (discriminant < 0.0f)
		return (0);
	t = (-abc[1] - sqrtf(discriminant)) / (2 * abc[0]);
	if (t < 0.0f || t > ray->closest_t)
		t = (-abc[1] + sqrtf(discriminant)) / (2 * abc[0]);
	if (t < 0.0f)
		return (0);
	if (t > ray->closest_t)
		return (0);
	ray->t = t;
	return (1);
}

int create_sphere(t_object *object, float radius)
{
	object->id = !PLANE;
	object->perturbator_index = 0;
	object->intersect = hit_sphere;
	object->normal = sphere_normal;
	object->uv_coords = sphere_uv_coords;
	object->size[0] = 0.0f;
	object->size[1] = radius;
	object->size[2] = 0.0f;
	object->orientation[0] = 0.0f;
	object->orientation[1] = 0.0f;
	object->orientation[2] = 1.0f;
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	set_basis(&object->basis, object->orientation);
	return (0);
}
