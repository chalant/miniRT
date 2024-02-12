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
	float		point[4];
	float		projected[4];
	float		up[3] = {1.0f, 0.0f, 0.0f};
	float		u[3];
	float		v[3];

	//todo: find a robust way of choosing the up vector
	cross_product(up, object->orientation, u);
	cross_product(u, object->orientation, v);
	subtract_vectors(hit->point, object->center, point, 3);
	projected[0] = dot_product(point, u, 3);
	projected[1] = dot_product(point, v, 3);
	projected[2] = dot_product(point, object->orientation, 3);
	uv_coords[0] = atan2f(projected[1], projected[0]) / (2.0f * M_PI);
	uv_coords[1] = (projected[2]) / object->size[1];
	return (uv_coords);
}

float	*cone_normal(t_object *object, t_hit *hit)
{
	float	point[3];
	float	cp[3];
	float	center[3];

	subtract_vectors(hit->ray_origin, object->center, center, 3);
	add_vectors(hit->point, center, point, 3);
	scale_vector(object->orientation, dot_product(object->orientation, point, 3), cp, 3);
	subtract_vectors(point, cp, hit->normal, 3);
	scale_vector(hit->normal, object->size[0], hit->normal, 3);
	normalize_vector(hit->normal, hit->normal, 3);
	return (hit->normal);
}

int hit_cone(t_object *object, t_ray *ray) 
{
	float abc[3];
	float ro;
	float oc;
	float vertex[3];
	float discriminant;
	float t;

	// Ensure direction vector is normalized

	// Find vector from ray origin to cone's apex
	subtract_vectors(ray->origin, object->center, vertex, 3);
	// Calculate terms for quadratic equation
	ro = dot_product(ray->direction, object->orientation, 3);
	oc = dot_product(vertex, object->orientation, 3);
	abc[0] = ro * ro - object->size[0] * object->size[0];
	abc[1] = 2.0f * (ro * oc - dot_product(ray->direction, vertex, 3) * object->size[0] * object->size[0]);
	abc[2] = oc * oc - dot_product(vertex, vertex, 3) * object->size[0] * object->size[0];

	// Calculate discriminant
	discriminant = abc[1] * abc[1] - 4.0f * abc[0] * abc[2];
	if (discriminant < 0.0f)
		return (0);
	discriminant = sqrtf(discriminant);
	float t1 = (-abc[1] - discriminant) / (2 * abc[0]);
	float t2 = (-abc[1] + discriminant) / (2 * abc[0]);
	float intersection[3];
	t = t1;
	if (t < 0.0f || (t2 >= 0.0f && t2 < t1))
		t = t2;
	if (t < 0.0f || t > ray->closest_t)
		return (0);
	scale_vector(ray->direction, t, intersection, 3);
	add_vectors(ray->origin, intersection, intersection, 3);
	subtract_vectors(intersection, object->center, intersection, 3);
	float height = dot_product(intersection, object->orientation, 3);
	if (height < 0.0f || height > object->size[1])
		return (0);
	ray->t = t;
	return 1;
}

int create_cone(t_object *object, float height, float radius)
{
	float	angle;

	angle = atanf(radius / height);
	object->intersect = hit_cone;
	object->normal = cone_normal;
	object->uv_coords = cone_uv_coords;
	object->size[0] = cosf(angle);
	object->size[1] = height;
	object->size[2] = 0.0f;
	object->orientation[0] = 0.0f;
	object->orientation[1] = -1.0f;
	object->orientation[2] = 0.0f;
	normalize_vector(object->orientation, object->orientation, 3);
	//scale_vector(object->orientation, -1.0f, object->orientation, 3);
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	set_basis(&object->basis, (float[3]){0.0f, 1.0f, 1.0f});
	return (0);
}
