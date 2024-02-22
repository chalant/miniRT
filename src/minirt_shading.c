/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_shading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:42:34 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/22 12:34:50 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	add_colors(t_minirt *minirt, t_hit *hit)
{
	int		j;
	float	color[4];

	j = -1;
	add_textures(minirt, hit, color);
	while (++j < minirt->lights.size)
		handle_light(minirt, hit,
			ft_darray_get(&minirt->lights, j), color);
	return (0);
}

void	add_sky_color(t_minirt *minirt, t_hit *hit)
{
	hit->color[0] += minirt->sky_color[0] * hit->energy;
	hit->color[1] += minirt->sky_color[1] * hit->energy;
	hit->color[2] += minirt->sky_color[2] * hit->energy;
}

int	init_hit(t_hit *hit, int *coords)
{
	hit->screen_coords = coords;
	hit->energy = 1.0f;
	to_color(0x00000000, hit->color);
	return (0);
}

int	set_hit_info(t_minirt *minirt, t_hit *hit, t_ray *ray)
{
	hit->material = ft_darray_get(&minirt->materials,
			hit->object->material_index);
	hit->ray_origin = ray->origin;
	scale_vector(ray->direction, ray->t, hit->point, 3);
	hit->object->normal(hit->object, hit);
	if (dot_product(hit->normal, ray->direction, 3) > 0.0f)
	{
		hit->inside = 1;
		scale_vector(hit->normal, -1.0f, hit->normal, 3);
	}
	add_vectors(hit->point, ray->origin, hit->point, 3);
	return (1);
}

int	shade_pixel(t_minirt *minirt, int coords[2])
{
	int		bounces;
	int		i;
	t_ray	ray;
	t_hit	hit;

	i = -1;
	bounces = 2;
	init_hit(&hit, coords);
	set_ray(minirt, &ray, coords[0], coords[1]);
	while (++i < bounces)
	{
		ray_trace(minirt, &ray, &hit, coords);
		if (hit.distance < 0.0f)
		{
			add_sky_color(minirt, &hit);
			break ;
		}
		set_hit_info(minirt, &hit, &ray);
		add_colors(minirt, &hit);
		bounce_ray(&ray, &hit);
	}
	minirt_pixel_put(&minirt->display, hit.screen_coords[0],
		hit.screen_coords[1], to_argb(hit.color));
	return (1);
}
