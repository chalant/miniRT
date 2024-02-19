/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_interaction.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:11:05 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:11:22 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	mouse_hit(t_minirt *minirt, t_ray *ray, t_object **closest)
{
	int			i;
	t_object	*object;

	*closest = NULL;
	ray->closest_t = FLT_MAX;
	i = -1;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		if (!object->intersect(object, ray))
			continue ;
		*closest = object;
		ray->closest_t = ray->t;
	}
	return (ray->closest_t < FLT_MAX);
}

int	grab_object(t_minirt *minirt, int x, int y)
{
	float		position[4];
	t_ray		ray;
	t_object	*closest;

	if (x < 0 || y < 0 || x > minirt->display.width
		|| y > minirt->display.height)
		return (0);
	to_screen_space(&minirt->display, ray.direction, x, y);
	to_world_space(minirt, ray.direction, position);
	copy_vector(minirt->camera.position, ray.origin, 3);
	matvec_product(&minirt->camera.inverse_view, position, ray.direction);
	if (!mouse_hit(minirt, &ray, &closest))
		return (0);
	minirt->selected_object = closest;
	minirt->mouse.hit_point = ray.closest_t;
	copy_vector(position, minirt->mouse.world_position, 3);
	return (1);
}
