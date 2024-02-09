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

float	*cylinder_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
{
	float		theta;
	float		phi;
	float		point[4];
	float		center[4];

	vmatmul(&object->basis, hit->point, point);
	vmatmul(&object->basis, object->center, center);
	phi = atan2f(point[2] - center[2], point[0] - center[0]);
	theta = acosf((point[1] - center[1]) / object->size[0]);
	uv_coords[0] = (phi + M_PI) / (2.0f * M_PI);
	uv_coords[1] = 1.0f - theta / M_PI;
	return (uv_coords);
}

float	*cylinder_normal(t_object *object, t_hit *hit)
{
	float		center[3];

	subtract_vectors(hit->ray_origin, hit->object->center, center, 3);
	add_vectors(hit->point, center, hit->normal, 3);
	scale_vector(hit->normal, 1 / object->size[0], hit->normal, 3);
	return (hit->normal);
}

int	hit_cylinder(t_object *object, t_ray *ray)
{
	float		a;
	float		b;
	float		c;
	float		discriminant;
	float		t;
	float		center[3];

	subtract_vectors(ray->origin, object->center, center, 3);
	a = dot_product(ray->direction, ray->direction, 3);
	b = 2.0f * dot_product(center, ray->direction, 3);
	c = dot_product(center, center, 3) - object->size[0] * object->size[0];
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0.0f)
		return (0);
	t = (-b - sqrt(discriminant)) / (2 * a);
	if (t < 0.0f || t > ray->closest_t)
		t = (-b + sqrt(discriminant)) / (2 * a);
	if (t < 0.0f)
		return (0);
	if (t > ray->closest_t)
		return (0);
	ray->t = t;
	return (1);
}

int create_cylinder(t_object *object, float radius)
{
	object->intersect = hit_cylinder;
	object->normal = cylinder_normal;
	object->uv_coords = cylinder_uv_coords;
	object->size[0] = radius;
	object->size[1] = 0.0f;
	object->size[2] = 0.0f;
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	set_basis(&object->basis, (float[3]){1.0f, 0.0f, 0.0f});
	return (0);
}
