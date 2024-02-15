/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_rendering.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:00:12 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:00:16 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	minirt_pixel_put(t_display *display, int x, int y, int color)
{
	char	*dst;

	if (x <= 0 || y <= 0)
		return ;
	if (x >= display->width || y >= display->height)
		return ;
	dst = display->addr + (y * display->line_length + x
			* display->offset);
	*(unsigned int *)dst = color;
}

float	*to_screen_space(t_display *display, float pixel[4], float i, float j)
{
	pixel[0] = (2.0f * i / (float)display->width) - 1.0f;
	pixel[1] = (2.0f * j / (float)display->height) - 1.0f;
	pixel[2] = 0.0f;
	pixel[3] = 1.0f;
	return (pixel);
}

int	set_hit_info(t_hit *hit, t_ray *ray)
{
	hit->ray_origin = ray->origin;
	scale_vector(ray->direction, hit->distance, hit->point, 3);
	hit->object->normal(hit->object, hit);
	add_vectors(hit->point, ray->origin, hit->point, 3);
	return (1);
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
			continue;
		ray->closest_t = ray->t;
		hit->distance = ray->t;
		hit->object = object;
		hit->screen_coords = coords;
	}
	return (1);
}

int	shadow_ray(t_minirt *minirt, t_ray *ray, t_hit *hit, t_object **closest)
{
	int			i;
	t_object	*object;

	i = -1;
	ray->closest_t = FLT_MAX;
	*closest = NULL;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		if (object == hit->object)
			continue;
		if (object->intersect(object, ray))
		{
			ray->closest_t = ray->t;
			*closest = object;
			return (1);
		}
	}
	return (0);
}

//result = I - 2.0 * dot(N, I) * N.
float	*reflect(float incident[3], float normal[3], float hit_angle, float result[3])
{
	float	tmp[3];

	scale_vector(normal, 2.0f * hit_angle, tmp, 3);
	subtract_vectors(incident, tmp, result, 3);
	return (result);
}

int	get_textures(t_hit *hit, float color[4])
{
	float		uv_coords[2];
	float		perturbation[3];

	hit->object->uv_coords(hit->object, hit, uv_coords);
	hit->object->material->normal_perturb(hit->object, uv_coords, perturbation);
	add_vectors(hit->normal, perturbation, hit->normal, 3);
	normalize_vector(hit->normal, hit->normal, 3);
	hit->object->material->get_texture(hit->object, uv_coords, color);
	return (0);
}

float	specular_power(t_minirt *minirt, t_hit *hit, t_light *light)
{
	float		reflection[3];
	float		view[3];
	float		light_direction[3];

	scale_vector(light->direction, -1.0f, light_direction, 3);
	reflect(light_direction, hit->normal, 
		dot_product(light_direction, hit->normal, 3), reflection);
	subtract_vectors(minirt->camera.origin, hit->point, view, 3);
	normalize_vector(view, view, 3);
	return (powf(fmaxf(0.0f, dot_product(reflection, view, 3)),
		hit->object->material->shininess) * light->brightness);
}

void	compute_shadows(t_minirt *minirt, t_hit *hit, t_light *light, float visibility[3])
{
	t_ray		lray;
	t_hit		lhit;
	t_object	*closest;

	copy_vector(hit->ray_origin, lray.origin, 3);
	copy_vector(light->direction, lray.direction, 3);
	lray.origin[0] = hit->point[0] + 0.001f * hit->normal[0];
	lray.origin[1] = hit->point[1] + 0.001f * hit->normal[1];
	lray.origin[2] = hit->point[2] + 0.001f * hit->normal[2];
	lhit.object = hit->object;
	shadow_ray(minirt, &lray, &lhit, &closest);
	visibility[0] = 1.0f;
	visibility[1] = 1.0f;
	visibility[2] = 1.0f;
	if (closest && lray.closest_t < FLT_MAX)
	{
		visibility[0] = 0.0f;
		visibility[1] = 0.2f;
		visibility[2] = 0.0f;
	}
}

float	ambient_light(t_minirt *minirt, t_hit *hit, int i)
{
	return (minirt->ambient.brightness * minirt->ambient.color[i] * hit->object->material->ambient_reflection);
}

float	diffuse_light(t_hit *hit, t_light *diffuse, int i)
{
	return (diffuse->brightness * diffuse->color[i] * hit->object->material->diffuse_reflection);
}

float	specular_light(t_hit *hit, t_light *diffuse, int i)
{
	return (hit->object->material->specular_reflection * diffuse->color[i]);
}

int	handle_light(t_minirt *minirt, t_hit *hit, t_light *light, float color[3])
{
	int			i;
	float		hit_angle;
	float		spec_pow;
	float		visibility[3];

	subtract_vectors(light->position, hit->point, light->direction, 3);
	add_vectors(hit->point, light->direction, light->direction, 3);
	normalize_vector(light->direction, light->direction, 3);
	hit_angle = dot_product(hit->normal, light->direction, 3);
	compute_shadows(minirt, hit, light, visibility);
	spec_pow = specular_power(minirt, hit, light);
	i = -1;
	while (++i < 3)
	{
		hit->color[i] += visibility[0] * color[i] * hit->energy
			* ambient_light(minirt, hit, i);
		hit->color[i] += visibility[1] * color[i] * hit->energy
			* hit_angle * diffuse_light(hit, light, i);
		hit->color[i] += visibility[2] *  hit->energy * spec_pow
			* specular_light(hit, light, i);
	}
	return (0);
}

float	*to_world_space(t_minirt *minirt, float point[4], float result[4])
{
	vmatmul(&minirt->world_space, point, result);
	scale_vector(result, 1 / result[3], result, 3);
	normalize_vector(result, result, 3);
	result[3] = 0.0f;
	return (result);
}

void	add_sky_color(t_minirt *minirt, t_hit *hit)
{
	hit->color[0] += minirt->sky_color[0] * hit->energy;
	hit->color[1] += minirt->sky_color[1] * hit->energy;
	hit->color[2] += minirt->sky_color[2] * hit->energy;
}

int	add_colors(t_minirt *minirt, t_hit *hit)
{
	int			j;
	float		color[4];

	j = -1;
	get_textures(hit, color);
	while (++j < minirt->spot_lights.size)
		handle_light(minirt, hit, ft_darray_get(&minirt->spot_lights, j), color);
	return (0);
}

void	set_ray(t_minirt *minirt, t_ray *ray, int coords[2])
{
	float		point[4];
	float		result[4];

	to_screen_space(&minirt->display, point, coords[0], coords[1]);
	to_world_space(minirt, point, result);
	vmatmul(&minirt->camera.inverse_view, result, ray->direction);
	copy_vector(minirt->camera.origin, ray->origin, 3);
}

void	bounce_ray(t_ray *ray, t_hit *hit)
{
	float		epsilon[3];

	add_vectors(hit->point, scale_vector(hit->normal, 0.0001f, epsilon, 3), ray->origin, 3);
	reflect(ray->direction, hit->normal,
			dot_product(ray->direction, hit->normal, 3), ray->direction);
	hit->energy *= hit->object->material->reflectivity;
}

int	shade_pixel(t_minirt *minirt, int coords[2])
{
	int			bounces;
	int			i;
	t_ray		ray;
	t_hit		hit;

	i = -1;
	bounces = 2;
	hit.screen_coords = coords;
	hit.energy = 1.0f;
	to_color(0x00000000, hit.color);
	set_ray(minirt, &ray, coords);
	while (++i < bounces)
	{
		ray_trace(minirt, &ray, &hit, coords);
		if (hit.distance < 0.0f)
		{
			add_sky_color(minirt, &hit);
			break;
		}
		set_hit_info(&hit, &ray);
		add_colors(minirt, &hit);
		bounce_ray(&ray, &hit);
	}
	minirt_pixel_put(&minirt->display, hit.screen_coords[0], hit.screen_coords[1], to_argb(hit.color));
	return (1);
}

int	render(t_minirt *minirt)
{
	int	coords[2];

	coords[0] = -1;
	while (++coords[0] < minirt->display.width)
	{
		coords[1] = -1;
		if ((coords[0] % 2))
			continue;
		while (++coords[1] < minirt->display.height)
			if (!(coords[1] % 2))
				shade_pixel(minirt, coords);
	}
	mlx_put_image_to_window(minirt->mlx, minirt->window, minirt->display.img, 0, 0);
	return (0);
}
