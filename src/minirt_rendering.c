#include "minirt.h"
#include <float.h>

void	copy_vector(float *src, float *dst, int n)
{
	while (--n > -1)
		dst[n] = src[n];
}

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

int	to_screen_space(t_display *display, float *pixel, float i, float j)
{
	pixel[0] = (2.0f * i / (float)display->width) - 1.0f;
	pixel[1] = (2.0f * j / (float)display->height) - 1.0f;
	pixel[2] = 1.0f;
	pixel[3] = 1.0f;
	return (0);
}

float	vector_length(float *vector, int size)
{
	int		i;
	float	length;

	i = -1;
	length = 0.0f;
	while (++i < size)
		length += vector[i] * vector[i];
	return (sqrtf(length));
}

int	set_hit_info(t_hit *hit, t_ray *ray)
{
	hit->ray_origin = ray->origin;
	scale_vector(ray->direction, hit->distance, hit->point, 3);
	hit->object->normal(hit->object, hit);
	add_vectors(hit->point, ray->origin, hit->point, 3);
	return (1);
}

//todo: perform ray intersection with all objects in the scene.
//todo: should use an abstract data structure for this.
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

int	shadow_ray(t_minirt *minirt, t_ray *ray, t_hit *hit)
{
	int			i;
	t_object	*object;

	i = -1;
	hit->object = NULL;
	ray->closest_t = FLT_MAX;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		if (object->intersect(object, ray))
		{
			ray->closest_t = ray->t;
			hit->object = object;
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

void	add_spot_lights(t_minirt *minirt, t_hit *hit, float view[3])
{
	int			i;
	int			j;
	float		specular_power;
	float		reflection[3];
	t_light		*light;
	float		direction[3];

	i = -1;
	while (++i < minirt->spot_lights.size)
	{
		j = -1;
		light = ft_darray_get(&minirt->spot_lights, i);
		subtract_vectors(hit->point, light->position, direction, 3);
		normalize_vector(direction, direction, 3);
		reflect(direction, hit->normal, dot_product(direction, hit->normal, 3), reflection);
		specular_power = powf(fmaxf(0.0f, dot_product(reflection, view, 3)),
			hit->object->material->shininess) * light->brightness;
		while (++j < 3)
			hit->color[j] += 0.5f * specular_power * light->color[j];
	}
}

float	distance(float *v1, float *v2, float *result, int n)
{
	subtract_vectors(v1, v2, result, n);
	return (vector_magnitude(result, n));
}

int	add_lights(t_minirt *minirt, t_ray *ray, t_hit *hit, float multiplier)
{
	int			i;
	float		view[3];
	float		hit_angle;
	float		direction[3];
	float		reflection[3];
	float		specular_power;
	float		uv_coords[2];
	float		color[4];
	float		perturbation[3];
	//float		normal[3];
	float		vis;
	t_ray		lray;
	t_hit		lhit;

	copy_vector(ray->origin, lray.origin, 3);
	subtract_vectors(minirt->diffuse.position, hit->point, direction, 3);
	normalize_vector(direction, direction, 3);
	hit->object->uv_coords(hit->object, hit, uv_coords);
	hit->object->material->normal_perturb(hit->object, uv_coords, perturbation);
	add_vectors(hit->normal, perturbation, hit->normal, 3);
	normalize_vector(hit->normal, hit->normal, 3);
	hit_angle = dot_product(hit->normal, direction, 3);
	lray.origin[0] = hit->point[0] + 0.0001f * hit->normal[0];
	lray.origin[1] = hit->point[1] + 0.0001f * hit->normal[1];
	lray.origin[2] = hit->point[2] + 0.0001f * hit->normal[2];
	copy_vector(direction, lray.direction, 3);
	vis = 1.0f;
	float	svis = 1.0f;
	float	avis = 1.0f;
	shadow_ray(minirt, &lray, &lhit);
	if (lray.closest_t < FLT_MAX && lhit.object != hit->object)
	{
		avis = 0.0f;
		vis = 0.2f;
		svis = 0.0f;
	}
	subtract_vectors(minirt->camera.origin, hit->point, view, 3);
	normalize_vector(view, view, 3);
	scale_vector(direction, -1.0f, direction, 3);
	reflect(direction, hit->normal, dot_product(direction, hit->normal, 3), reflection) ;
	specular_power = powf(fmaxf(0.0f, dot_product(reflection, view, 3)),
		hit->object->material->shininess) * minirt->diffuse.brightness;
	i = -1;
	hit->object->material->get_texture(hit->object, uv_coords, color);
	while (++i < 3)
	{
		hit->color[i] += avis * minirt->ambient.brightness * minirt->ambient.color[i] * color[i] * hit->object->material->ambient_reflection * multiplier;
		hit->color[i] += vis * hit_angle * minirt->diffuse.brightness * minirt->diffuse.color[i] * color[i] * hit->object->material->diffuse_reflection * multiplier;
		hit->color[i] += svis * 0.9f * specular_power * minirt->diffuse.color[i] * multiplier;
	}
	return (0);
}

int	shade_pixel(t_minirt *minirt, int coords[2])
{
	float		point[4];
	float		result[4];
	t_ray		ray;
	t_hit		hit;
	int			bounces;
	int			i;
	float		epsilon[3];
	float		sky_color[4];

	i = -1;
	bounces = 2;
	to_screen_space(&minirt->display, point, coords[0], coords[1]);
	vmatmul(minirt->world_space, point, result);
	scale_vector(result, 1 / result[3], result, 3);
	normalize_vector(result, result, 3);
	result[3] = 0.0f;
	vmatmul(minirt->camera.inverse_view, result, ray.direction);
	to_color(0x00000000, hit.color);
	//minirt_pixel_put(&minirt->display, coords[0], coords[1], 0x0);
	copy_vector(minirt->camera.origin, ray.origin, 3);
	hit.screen_coords = coords;
	float	multiplier = 1.0f;
	to_color(0x00000000, sky_color);
	// sky_color[0] = 0.1f;
	// sky_color[1] = 0.8f;
	// sky_color[2] = 0.6f;
	while (++i < bounces)
	{
		ray_trace(minirt, &ray, &hit, coords);
		if (hit.distance < 0.0f)
		{
			hit.color[0] += sky_color[0] * multiplier;
			hit.color[1] += sky_color[1] * multiplier;
			hit.color[2] += sky_color[2] * multiplier;
			break;
		}
		set_hit_info(&hit, &ray);
		add_vectors(hit.point, scale_vector(hit.normal, 0.0001f, epsilon, 3), ray.origin, 3);
		add_lights(minirt, &ray, &hit, multiplier);
		reflect(ray.direction, hit.normal, dot_product(ray.direction, hit.normal, 3), ray.direction);
		multiplier *= hit.object->material->reflectivity;
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
		// if ((coords[0] % 2))
		// 	continue;
		while (++coords[1] < minirt->display.height)
			//if (!(coords[1] % 2))
				shade_pixel(minirt, coords);
	}
	mlx_put_image_to_window(minirt->mlx, minirt->window, minirt->display.img, 0, 0);
	return (0);
}
