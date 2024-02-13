/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:05:06 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:05:07 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*choose_arbitray_vector(float result[3], float axis[3])
{
	result[0] = 0.0f;
	result[1] = 1.0f;
	result[2] = 0.0f;
	if (dot_product(axis, result, 3) == 1.0f)
	{
		result[0] = 0.0f;
		result[1] = 0.0f;
		result[2] = 1.0f;
	}
	if (dot_product(axis, result, 3) == 1.0f)
	{
		result[0] = 1.0f;
		result[1] = 0.0f;
		result[2] = 0.0f;
	}
	return (result);
}

void	create_basis(float orientation[3], float u[3], float v[3])
{
	float	arbitrary[3];

	choose_arbitray_vector(arbitrary, orientation);
	cross_product(arbitrary, orientation, u);
	cross_product(u, orientation, v);
}

float	*cylinder_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
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
	uv_coords[0] = atan2f(projected[1], projected[0]) / (2.0f * M_PI);
	uv_coords[1] = (projected[2]) / object->size[1];
	return (uv_coords);
}

float	*cylinder_normal(t_object *object, t_hit *hit)
{
	float	point[3];
	float	cp[3];
	float	center[3];

	subtract_vectors(hit->ray_origin, object->center, center, 3);
	add_vectors(hit->point, center, point, 3);
	scale_vector(object->orientation, dot_product(object->orientation, point, 3), cp, 3);
	subtract_vectors(point, cp, hit->normal, 3);
	scale_vector(hit->normal, 1 / object->size[0], hit->normal, 3);
	//normalize_vector(hit->normal, hit->normal, 3);
	return (hit->normal);
}

// int	hit_cylinder(t_object *object, t_ray *ray)
// {
// 	float	c1[3];
// 	float	c2[3];
// 	float	abc[3];
// 	float	axis[3];
// 	float	oc[3];
// 	float	baoc;
// 	float	dd;
// 	float	t;

// 	scale_vector(object->orientation, object->size[1] / 2.0f, c2, 3);
// 	add_vectors(object->center, c2, c2, 3);
// 	scale_vector(object->orientation, object->size[1] / 2.0f, c1, 3);
// 	subtract_vectors(object->center, c1, c1, 3);
// 	subtract_vectors(c2, c1, axis, 3);
// 	subtract_vectors(ray->origin, c1, oc, 3);
// 	dd = dot_product(axis, axis, 3);
// 	baoc = dot_product(oc, axis, 3);
// 	float	bard = dot_product(ray->direction, axis, 3);
// 	abc[0] = dd * dot_product(oc, oc, 3) - baoc * baoc - object->size[0] * object->size[0] * dd;
// 	abc[1] = dd * dot_product(oc, ray->direction, 3) - baoc * bard;
// 	abc[2] = dd - bard * bard;
// 	float	det = abc[1] * abc[1] - abc[2] * abc[0];
// 	if (det < 0.0f)
// 		return (0);
// 	float	t1 = (-abc[1] - sqrtf(det)) / abc[2];
// 	float	t2 = (-abc[1] + sqrtf(det)) / abc[2];
// 	float intersection[3];
// 	t = t1;
// 	scale_vector(ray->direction, t, intersection, 3);
// 	add_vectors(ray->origin, intersection, intersection, 3);
// 	//scale_vector(ray->direction, t, intersection, 3);
// 	subtract_vectors(intersection, object->center, intersection, 3);
// 	float height = dot_product(intersection, object->orientation, 3);
// 	if (height < 0.0f || height > object->size[1] || t < 0.0f || t > ray->closest_t)
// 		t = t2;
// 	if (t < 0.0f || t > ray->closest_t)
// 		return (0);
// 	scale_vector(ray->direction, t, intersection, 3);
// 	add_vectors(ray->origin, intersection, intersection, 3);
// 	//scale_vector(ray->direction, t, intersection, 3);
// 	subtract_vectors(intersection, object->center, intersection, 3);
// 	height = dot_product(intersection, object->orientation, 3);
// 	if (height < 0.0f || height > object->size[1])
// 		return (0);
// 	ray->t = t;
// 	return 1;
// }

int	hit_cylinder(t_object *object, t_ray *ray)
{
	float	oc[3];
	float	rd_axis;
	float	oc_axis;
	float	abc[3];

	subtract_vectors(ray->origin, object->center, oc, 3);
	rd_axis = dot_product(ray->direction, object->orientation, 3);
	oc_axis = dot_product(oc, object->orientation, 3);
	abc[0] = 1.0f - rd_axis * rd_axis;
	abc[1] = dot_product(ray->direction, oc, 3) - rd_axis * oc_axis;
	abc[2] = dot_product(oc, oc, 3) - oc_axis * oc_axis - object->size[0] * object->size[0];

	float	det = abc[1] * abc[1] - abc[0] * abc[2];
	if (det < 0.0f)
		return (0);
	float	t1 = (-abc[1] - sqrtf(det)) / abc[0];
	float	t2 = (-abc[1] + sqrtf(det)) / abc[0];
	float intersection[3];
	float	t = t1;
	if (t < 0.0f || (t2 > 0.0f && t2 < t1))
		t = t2;
	// else
	// 	t = t1;
	if (t < 0.0f || t > ray->closest_t)
		return (0);
	scale_vector(ray->direction, t, intersection, 3);
	add_vectors(ray->origin, intersection, intersection, 3);
	subtract_vectors(intersection, object->center, intersection, 3);
	float height = dot_product(intersection, object->orientation, 3);
	if (height < 0.0f || height > object->size[1] || t > ray->closest_t || t < 0.0f)
		t = t2;
	if (t < 0.0f || t > ray->closest_t)
		return (0);
	scale_vector(ray->direction, t, intersection, 3);
	add_vectors(ray->origin, intersection, intersection, 3);
	subtract_vectors(intersection, object->center, intersection, 3);
	height = dot_product(intersection, object->orientation, 3);
	if (height < 0.0f || height > object->size[1] || t > ray->closest_t || t < 0.0f)
		return (0);
	ray->t = t;
	return 1;
}

int create_cylinder(t_object *object, float height, float radius)
{
	object->intersect = hit_cylinder;
	object->normal = cylinder_normal;
	object->uv_coords = cylinder_uv_coords;
	object->size[0] = radius;
	object->size[1] = height;
	object->size[2] = 0.0f;
	object->orientation[0] = 0.0f;
	object->orientation[1] = 1.0f;
	object->orientation[2] = 0.0f;
	normalize_vector(object->orientation, object->orientation, 3);
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	set_basis(&object->basis, (float[3]){0.0f, 0.0f, 1.0f});
	return (0);
}
