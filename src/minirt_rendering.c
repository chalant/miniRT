#include "minirt.h"
#include <float.h>

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
	return (0);
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
	//fprintf(stderr, "ray.origin: %f, %f, %f\n", ray->origin[0], ray->origin[1], ray->origin[2]);
	while (++i < minirt->objects->size)
	{
		object = ft_darray_get(minirt->objects, i);
		if (!object->intersect(object, ray))
			continue;
		ray->closest_t = ray->t;
		hit->distance = ray->t;
		hit->object = object;
		hit->screen_coords = coords;
	}
	return (1);
}

//result = I - 2.0 * dot(N, I) * N.
float	*reflect(float incident[3], float normal[3], float hit_angle, float result[3])
{
	float	tmp[3];

	scale_vector(normal, 2.0f * hit_angle, tmp, 3);
	subtract_vectors(incident, tmp, result, 3);
	return (result);
}

float	get_specular_light(t_light *light, t_hit *hit, float *view)
{
	float		reflection[3];
	float		specular_angle;
	float		specular_power;

	reflect(light->direction, hit->normal, dot_product(light->direction, hit->normal, 3), reflection);
	normalize_vector(view, view, 3);
	specular_angle = dot_product(reflection, view, 3);
	specular_power = powf(specular_angle, hit->object->material->shininess);
	return (specular_power * light->brightness);
}

void	add_spot_lights(t_minirt *minirt, t_hit *hit, float *hit_color)
{
	int			i;
	int			j;
	float		view[3];
	float		specular_power;
	t_light		*light;

	subtract_vectors(minirt->camera->origin, hit->point, view, 3);
	normalize_vector(view, view, 3);
	i = -1;
	while (++i < minirt->spot_lights->size)
	{
		light = ft_darray_get(minirt->spot_lights, i);
		specular_power = get_specular_light(light, hit, view);
		j = -1;
		while (++j < 3)
			hit_color[j] += specular_power * light->color[j];
	}
}

int	shade_pixel(t_minirt *minirt, int coords[2])
{
	float		point[4];
	float		result[4];
	float		hit_color[4];
	float		hit_angle;
	t_ray		ray;
	t_hit		hit;
	int			bounces;
	int			i;
	float		epsilon[3];
	float		origin[3];
	float		sky_color[3];

	i = -1;
	hit_angle = 0.0f;
	bounces = 2;
	to_screen_space(minirt->display, point, coords[0], coords[1]);
	vmatmul(minirt->world_space, point, result);
	scale_vector(result, 1 / result[3], result, 3);
	normalize_vector(result, result, 3);
	result[3] = 0.0f;
	vmatmul(minirt->camera->inverse_view, result, ray.direction);
	to_color(0x000000, hit_color);
	minirt_pixel_put(minirt->display, coords[0], coords[1], 0x0);
	origin[0] =  minirt->camera->origin[0];
	origin[1] =  minirt->camera->origin[1];
	origin[2] =  minirt->camera->origin[2];
	hit.screen_coords = coords;
	ray.origin = origin;
	float	multiplier = 1.0f;
	sky_color[0] = 0.0f;
	sky_color[1] = 0.0f;
	sky_color[2] = 0.0f;
	int	j;
	while (++i < bounces)
	{
		j = -1;
		ray_trace(minirt, &ray, &hit, coords);
		if (hit.distance < 0.0f)
		{
			hit_color[0] += sky_color[0] * multiplier;
			hit_color[1] += sky_color[1] * multiplier;
			hit_color[2] += sky_color[2] * multiplier;
			break;
		}
		set_hit_info(&hit, &ray);
		hit_angle = dot_product(hit.normal, minirt->diffuse.direction, 3);
		while (++j < 3)
		{
			//hit_color[i] += minirt->ambient.brightness * minirt->ambient.color[i];
			hit_color[j] += hit_angle * minirt->diffuse.brightness * minirt->diffuse.color[j] * hit.object->color[j] * multiplier;
		}
		hit_color[3] = 0.0f;
		//add_spot_lights(minirt, &hit, hit_color);
		ray.origin = hit.point;
		add_vectors(ray.origin, scale_vector(hit.normal, 0.0001f, epsilon, 3), ray.origin, 3);
		reflect(ray.direction, hit.normal, dot_product(ray.direction, hit.normal, 3), ray.direction);
		//todo: need shadows
		multiplier *= 0.3f;
	}
	minirt_pixel_put(minirt->display, hit.screen_coords[0], hit.screen_coords[1], to_argb(hit_color));
	return (1);
}

int	render(t_minirt *minirt)
{
	int			coords[2];

	coords[0] = -1;
	while (++coords[0] < minirt->display->width)
	{
		coords[1] = -1;
		// if ((coords[0] % 2))
		// 	continue;
		while (++coords[1] < minirt->display->height)
			//if (!(coords[1] % 2))
				shade_pixel(minirt, coords);
	}
	mlx_put_image_to_window(minirt->mlx, minirt->window, minirt->display->img, 0, 0);
	return (0);
}
