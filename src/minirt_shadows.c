/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_shadows.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:23:27 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 17:40:56 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	shadow_ray(t_minirt *minirt, t_ray *ray, t_object **closest)
{
	int			i;
	t_object	*object;

	i = -1;
	ray->closest_t = FLT_MAX;
	*closest = NULL;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		if (object->intersect(object, ray))
		{
			ray->closest_t = ray->t;
			*closest = object;
			return (1);
		}
	}
	return (0);
}

void	cast_shadows(t_minirt *minirt, t_hit *hit,
	t_light *light, float visibility[3])
{
	t_ray		sray;
	t_object	*closest;

	copy_vector(hit->ray_origin, sray.origin, 3);
	copy_vector(light->direction, sray.direction, 3);
	sray.origin[0] = hit->point[0] + 0.01f * hit->normal[0];
	sray.origin[1] = hit->point[1] + 0.01f * hit->normal[1];
	sray.origin[2] = hit->point[2] + 0.01f * hit->normal[2];
	shadow_ray(minirt, &sray, &closest);
	visibility[0] = 1.0f;
	visibility[1] = 1.0f;
	visibility[2] = 1.0f;
	if (closest && sray.closest_t < FLT_MAX && sray.closest_t >= 0.0f)
	{
		visibility[0] = 0.0f;
		visibility[1] = 0.2f;
		visibility[2] = 0.0f;
	}
}
