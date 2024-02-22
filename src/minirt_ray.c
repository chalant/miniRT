/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_ray.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:20:34 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/22 12:33:55 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//result = I - 2.0 * dot(N, I) * N.
float	*reflect(float incident[3], float normal[3],
	float hit_angle, float result[3])
{
	float	tmp[3];

	scale_vector(normal, 2.0f * hit_angle, tmp, 3);
	subtract_vectors(incident, tmp, result, 3);
	return (result);
}

void	set_ray(t_minirt *minirt, t_ray *ray, int x, int y)
{
	float	point[4];
	float	result[4];

	to_screen_space(&minirt->display, point, x, y);
	to_world_space(minirt, point, result);
	matvec_product(&minirt->camera.inverse_view, result, ray->direction);
	copy_vector(minirt->camera.position, ray->origin, 3);
}

void	bounce_ray(t_ray *ray, t_hit *hit)
{
	float	epsilon[3];

	add_vectors(hit->point, scale_vector(hit->normal, 0.01f, epsilon, 3),
		ray->origin, 3);
	reflect(ray->direction, hit->normal,
		dot_product(ray->direction, hit->normal, 3), ray->direction);
	hit->energy *= hit->material->reflectivity;
}

int	ray_trace(t_minirt *minirt, t_ray *ray, t_hit *hit, int coords[2])
{
	int			i;
	t_object	*object;

	i = -1;
	hit->distance = -1.0f;
	ray->closest_t = FLT_MAX;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		if (!object->intersect(object, ray))
			continue ;
		ray->closest_t = ray->t;
		hit->distance = ray->t;
		hit->object = object;
		hit->screen_coords = coords;
	}
	return (1);
}
