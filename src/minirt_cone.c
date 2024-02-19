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

float	*cone_normal(t_object *object, t_hit *hit)
{
	float	pc[3];
	float	tmp[3];

	add_vectors(hit->ray_origin, hit->point, tmp, 3);
	subtract_vectors(tmp, object->center, pc, 3);
	cross_product(object->orientation, pc, tmp);
	cross_product(tmp, pc, hit->normal);
	normalize_vector(hit->normal, hit->normal, 3);
	return (hit->normal);
}

int	solve_cone(t_object *object, t_ray *ray, float solutions[2])
{
	float	k;
	float	abc[3];
	float	ab[3];
	float	oc[3];
	float	discriminant;

	k = 1.0f + object->size[0] * object->size[0];
	scale_vector(object->orientation, object->size[1], ab, 3);
	subtract_vectors(ray->origin, object->center, oc, 3);
	abc[0] = dot_product(ray->direction, ray->direction, 3) - k
		* powf(dot_product(ray->direction, object->orientation, 3), 2);
	abc[1] = 2 * (dot_product(ray->direction, oc, 3) - k
			* dot_product(ray->direction, object->orientation, 3)
			* dot_product(oc, object->orientation, 3));
	abc[2] = dot_product(oc, oc, 3) - k
		* powf(dot_product(oc, object->orientation, 3), 2);
	discriminant = abc[1] * abc[1] - 4.0f * abc[0] * abc[2];
	if (discriminant < 0.0f)
		return (0);
	discriminant = sqrtf(discriminant);
	solutions[0] = (-abc[1] - discriminant) / (2 * abc[0]);
	solutions[1] = (-abc[1] + discriminant) / (2 * abc[0]);
	return (2);
}

float	project_point(t_ray *ray, t_object *object, float hit)
{
	float	projection[3];

	scale_vector(ray->direction, hit, projection, 3);
	add_vectors(ray->origin, projection, projection, 3);
	subtract_vectors(object->center, projection, projection, 3);
	return (dot_product(projection, object->orientation, 3));
}

int	hit_cone(t_object *object, t_ray *ray)
{
	float	solutions[2];
	float	height;
	float	t;
	float	n_solutions;

	n_solutions = solve_cone(object, ray, solutions);
	if (!n_solutions)
		return (0);
	t = solutions[0];
	height = project_point(ray, object, t);
	if (height < 0.0f || height > object->size[1]
		|| t > ray->closest_t || t < 0.0f)
		t = solutions[1];
	if (t < 0.0f || t > ray->closest_t)
		return (0);
	height = project_point(ray, object, t);
	if (height < 0.0f || height > object->size[1])
		return (0);
	ray->t = t;
	return (1);
}

int	create_cone(t_object *object, float height, float diameter)
{
	object->id = !PLANE;
	object->intersect = hit_cone;
	object->normal = cone_normal;
	object->uv_coords = cone_uv_coords;
	object->size[0] = tanf(diameter / 2.0f / height);
	object->size[1] = height;
	object->size[2] = 0.0f;
	normalize_vector(object->orientation, object->orientation, 3);
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	set_basis(&object->basis, object->orientation);
	return (0);
}
