/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_lights.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:15 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 17:43:16 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	specular_power(t_minirt *minirt, t_hit *hit, t_light *light)
{
	float	reflection[3];
	float	view[3];
	float	light_direction[3];

	scale_vector(light->direction, -1.0f, light_direction, 3);
	reflect(light_direction, hit->normal,
		dot_product(light_direction, hit->normal, 3), reflection);
	subtract_vectors(minirt->camera.position, hit->point, view, 3);
	normalize_vector(view, view, 3);
	return (powf(fmaxf(0.0f, dot_product(reflection, view, 3)),
			hit->material->shininess) * light->brightness);
}

float	ambient_light(t_minirt *minirt, t_hit *hit, int i)
{
	return (minirt->ambient.brightness * minirt->ambient.color[i]
		* hit->material->ambient_reflection);
}

float	diffuse_light(t_hit *hit, t_light *diffuse, int i)
{
	return (diffuse->brightness * diffuse->color[i]
		* hit->material->diffuse_reflection);
}

float	specular_light(t_hit *hit, t_light *diffuse, int i)
{
	return (hit->material->specular_reflection * diffuse->color[i]);
}

int	handle_light(t_minirt *minirt, t_hit *hit, t_light *light, float color[3])
{
	int		i;
	float	hit_angle;
	float	spec_pow;
	float	visibility[3];

	subtract_vectors(light->position, hit->point, light->direction, 3);
	add_vectors(hit->point, light->direction, light->direction, 3);
	normalize_vector(light->direction, light->direction, 3);
	hit_angle = dot_product(hit->normal, light->direction, 3);
	cast_shadows(minirt, hit, light, visibility);
	spec_pow = specular_power(minirt, hit, light);
	i = -1;
	while (++i < 3)
	{
		hit->color[i] += visibility[0] * color[i] * hit->energy
			* ambient_light(minirt, hit, i);
		hit->color[i] += visibility[1] * color[i] * hit->energy
			* hit_angle * diffuse_light(hit, light, i);
		hit->color[i] += visibility[2] * hit->energy * spec_pow
			* specular_light(hit, light, i);
	}
	return (0);
}
