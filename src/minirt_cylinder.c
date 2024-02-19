/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:05:06 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 17:05:12 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	scale_vector(object->orientation,
		dot_product(object->orientation, point, 3), cp, 3);
	subtract_vectors(point, cp, hit->normal, 3);
	scale_vector(hit->normal, 1 / object->size[0], hit->normal, 3);
	return (hit->normal);
}

int	solve_cylinder(t_object *object, t_ray *ray, float solutions[2])
{
	float	oc[3];
	float	rd_axis;
	float	oc_axis;
	float	abc[3];
	float	det;

	subtract_vectors(ray->origin, object->center, oc, 3);
	rd_axis = dot_product(ray->direction, object->orientation, 3);
	oc_axis = dot_product(oc, object->orientation, 3);
	abc[0] = 1.0f - rd_axis * rd_axis;
	abc[1] = dot_product(ray->direction, oc, 3) - rd_axis * oc_axis;
	abc[2] = dot_product(oc, oc, 3) - oc_axis
		* oc_axis - object->size[0] * object->size[0];
	det = abc[1] * abc[1] - abc[0] * abc[2];
	if (det < 0.0f)
		return (0);
	solutions[0] = (-abc[1] - sqrtf(det)) / abc[0];
	solutions[1] = (-abc[1] + sqrtf(det)) / abc[0];
	return (2);
}

int	hit_cylinder(t_object *object, t_ray *ray)
{
	float	t;
	float	solutions[2];
	float	n_solutions;
	float	height;

	n_solutions = solve_cylinder(object, ray, solutions);
	if (!n_solutions)
		return (0);
	t = solutions[0];
	height = project_point(ray, object, t);
	if (t > ray->closest_t || t < 0.0f
		|| height < 0.0f || height > object->size[1])
		t = solutions[1];
	if (t < 0.0f || t > ray->closest_t)
		return (0);
	height = project_point(ray, object, t);
	if (height < 0.0f || height > object->size[1])
		return (0);
	ray->t = t;
	return (1);
}

int	create_cylinder(t_object *object, float height, float diameter)
{
	object->id = !PLANE;
	object->perturbator_index = 0;
	object->intersect = hit_cylinder;
	object->normal = cylinder_normal;
	object->uv_coords = cylinder_uv_coords;
	object->size[0] = diameter / 2.0f;
	object->size[1] = height;
	object->size[2] = 0.0f;
	normalize_vector(object->orientation, object->orientation, 3);
	if (homogeneous_matrix(&object->basis, 3, 3) == -1)
		return (-1);
	set_basis(&object->basis, object->orientation);
	return (0);
}
