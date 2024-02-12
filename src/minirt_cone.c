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

// int hit_cone(t_object *object, t_ray *ray) 
// {
// 	float abc[3];
// 	float ro;
// 	float oc;
// 	float vertex[3];
// 	float discriminant;
// 	float t;

// 	// Ensure direction vector is normalized

// 	// Find vector from ray origin to cone's apex
// 	subtract_vectors(ray->origin, object->center, vertex, 3);
// 	// Calculate terms for quadratic equation
// 	ro = dot_product(ray->direction, object->orientation, 3);
// 	oc = dot_product(vertex, object->orientation, 3);
// 	abc[0] = ro * ro - object->size[0] * object->size[0];
// 	abc[1] = 2.0f * (ro * oc - dot_product(ray->direction, vertex, 3) * object->size[0] * object->size[0]);
// 	abc[2] = oc * oc - dot_product(vertex, vertex, 3) * object->size[0] * object->size[0];

// 	// Calculate discriminant
// 	discriminant = abc[1] * abc[1] - 4.0f * abc[0] * abc[2];
// 	if (discriminant < 0.0f)
// 		return (0);
// 	discriminant = sqrtf(discriminant);
// 	float t1 = (-abc[1] - discriminant) / (2 * abc[0]);
// 	float t2 = (-abc[1] + discriminant) / (2 * abc[0]);
// 	float intersection[3];
// 	t = t1;
// 	if (t < 0.0f || (t2 >= 0.0f && t2 < t1))
// 		t = t2;
// 	if (t < 0.0f || t > ray->closest_t)
// 		return (0);
// 	scale_vector(ray->direction, t, intersection, 3);
// 	add_vectors(ray->origin, intersection, intersection, 3);
// 	subtract_vectors(intersection, object->center, intersection, 3);
// 	float height = dot_product(intersection, object->orientation, 3);
// 	if (height < 0.0f || height > object->size[1])
// 		return (0);
// 	ray->t = t;
// 	return 1;
// }

// int	hit_cone(t_object *object, t_ray *ray)
// {
// 	float	ba[3];
// 	float	oa[3];
// 	float	ob[3];
// 	float	ab[3];
// 	//float	abc[3];
// 	float	m0;
// 	float	m1;
// 	float	m2;
// 	float	m3;
// 	float	m5;
// 	float	m9;

// 	scale_vector(object->orientation, object->size[1], ab, 3);
// 	add_vectors(object->center, ab, ab, 3);
// 	subtract_vectors(ray->origin, ab, ob, 3);
// 	subtract_vectors(ray->origin, object->center, oa, 3);
// 	subtract_vectors(object->center, ab, ba, 3);
// 	m0 = dot_product(ray->direction, ray->direction, 3);
// 	m1 = dot_product(oa, ba, 3);
// 	m2 = dot_product(ray->direction, ba, 3);
// 	m3 = dot_product(ray->direction, oa, 3);
// 	m5 = dot_product(oa, oa, 3);
// 	m9 = dot_product(ob, ba, 3);
// 	float	rr = object->size[0];
//     float hy = m0 + rr*rr;
//     float k2 = m0*m0    - m2*m2*hy;
//     float k1 = m0*m0*m3 - m1*m2*hy;
//     float k0 = m0*m0*m5 - m1*m1*hy;
//     float h = k1*k1 - k2*k0;
// 	if (h < 0.0f)
// 		return (0);
// 	float t = (-k1-sqrt(h))/k2;
//     float y = m1 + t*m2;
// 	if( y<0.0 || y>m0)
// 		return (0);
// 	ray->t = t;
// 	return 1;
// }

// static	void	inter_cone(t_mini_rt *rt, t_element *cone, t_solve s,
// t_mini_rt rtt)
// {
// 	t_element	plan;
// 	t_vec		inter;
// 	float		t;

// 	t = INT_MAX;
// 	if (s.t1 >= 0 && rt->t > s.t1)
// 		t = s.t1;
// 	else if (s.t2 >= 0 && rt->t > s.t2)
// 		t = s.t2;
// 	if (t == INT_MAX)
// 		return ;
// 	inter = vec_add(rtt.ray.ori, vec_mul(rtt.ray.dir, t));
// 	plan.orient = cone->orient;
// 	plan.point = cone->point;
// 	rtt.t = INT_MAX;
// 	plane(&rtt, &plan, inter, vec_normalize(cone->orient));
// 	rtt.t <= cone->height ? rt->t = t : 0;
// }

// void	plane(t_mini_rt *rt, t_element *plane, t_vec ori, t_vec dir)
// {
// 	t_solve	s;

// 	s.a = vec_dot(vec_sub(ori, plane->point), plane->orient);
// 	s.b = vec_dot(dir, plane->orient);
// 	if (s.b == 0 || (s.a < 0 && s.b < 0) || (s.a > 0 && s.b > 0))
// 		return ;
// 	s.t1 = -s.a / s.b;
// 	if (s.t1 < 0 || rt->t < s.t1)
// 		return ;
// 	rt->t = s.t1;
// }

int	hit_cone(t_object *object, t_ray *ray)
{
	float	ab[3];
	float	abc[3];
	float	oc[3];
	float	direction[3];
	
	float	k = object->size[0];
	normalize_vector(ray->direction, direction, 3);
	scale_vector(object->orientation, object->size[1], ab, 3);
	subtract_vectors(ray->origin, object->center, oc, 3);
	abc[0] = dot_product(direction, direction, 3) - (1 + k * k) *  powf(dot_product(direction, object->orientation, 3), 2);
	abc[1] = 2 * (dot_product(direction, oc, 3) - (1 + k * k) * dot_product(direction, object->orientation, 3) * dot_product(oc, object->orientation, 3));
	abc[2] = dot_product(oc, oc, 3) - (1 + k * k) * powf(dot_product(oc, object->orientation, 3), 2);
	float	discriminant = abc[1] * abc[1] - 4.0f * abc[0] * abc[2];
	if (discriminant < 0.0f)
		return (0);
	discriminant = sqrtf(discriminant);
	float t1 = (-abc[1] - discriminant) / (2 * abc[0]);
	float t2 = (-abc[1] + discriminant) / (2 * abc[0]);
	//printf("Error %f %f %f\n", discriminant, t1, t2);
	float	t = t1;
	if (t < 0.0f || (t2 > 0.0f && t2 < t1))
		t = t2;
	if (t < 0.0f || t > ray->closest_t)
		return (0);
	float intersection[3];
	scale_vector(direction, t, intersection, 3);
	add_vectors(ray->origin, intersection, intersection, 3);
	subtract_vectors(intersection, object->center, intersection, 3);
	float height = dot_product(intersection, object->orientation, 3);
	if (height < 0.0f || height > object->size[1])
	{
		printf("HELLO %f\n", height);
		return (0);
	}
	//printf("TEST %f\n", height);
	ray->t = t;
	return 1;
}

int create_cone(t_object *object, float height, float radius)
{
	// float	angle;

	// angle = atanf(radius / height);
	object->intersect = hit_cone;
	object->normal = cone_normal;
	object->uv_coords = cone_uv_coords;
	object->size[0] = tanf(radius / height);
	object->size[1] = height;
	object->size[2] = 0.0f;
	object->orientation[0] = 0.0f;
	object->orientation[1] = 1.0f;
	object->orientation[2] = 0.0f;
	normalize_vector(object->orientation, object->orientation, 3);
	scale_vector(object->orientation, -1.0f, object->orientation, 3);
	if (homogeneous_matrix(&object->basis, 3, 3) < 0)
		return (-1);
	set_basis(&object->basis, (float[3]){0.0f, 1.0f, 1.0f});
	return (0);
}
