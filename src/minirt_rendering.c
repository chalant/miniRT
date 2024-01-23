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

void	print_matrix(t_matrix *matrix)
{
	int	i;
	int	j;

	i = -1;
	while (++i < matrix->rows)
	{
		j = -1;
		while (++j < matrix->cols)
			fprintf(stderr, "%f ", matrix->points[i][j]);
		fprintf(stderr, "\n");
	}
}

int	init_ray(t_ray *ray)
{
	ft_memset(ray->direction, 0, sizeof(float) * 4);
	ft_memset(ray->object_center, 0, sizeof(float) * 4);
	ray->t = 0.0f;
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

//todo: perform ray intersection with all objects in the scene.
int	draw_pixel(t_minirt *minirt, t_display *display, t_camera *camera, int x ,int y)
{
	int			i;
	float		result[4];
	float		point[4];
	t_object	*object;
	t_object	*closest;
	t_ray		ray;
	//float		min_hit;
	float		hit_color[4];
	float		hit_angle = 0.0f;
	float		cx;
	float		cy;
	float		normal[4];

	to_screen_space(minirt->display, point, x, y);
	vmatmul(minirt->world_space, point, result);
	scale_vector(result, 1 / result[3], 3);
	normalize_vector(result, 3);
	//fprintf(stderr, "%f\n", result[2]);
	result[3] = 0.0f;
	//fprintf(stderr, "result %f %f %f\n", result[0], result[1], result[2]);
	//puts it back to world space.
	vmatmul(camera->inverse_view, result, ray.direction);
	//moves point to world camera space (view space)
	//todo: need to multiply the basis by the camera translation.
	//vmatmul(camera->view, result, ray.direction);
	//fprintf(stderr, "direction %f %f %f\n", ray.direction[0], ray.direction[1], ray.direction[2]);
	//vmatmul(camera->inverse_transform, minirt->light->direction, light_direction);
	//min_hit = FLT_MAX;
	minirt_pixel_put(display, x, y, 0x0);
	ray.transform = camera->inverse_view;
	ray.inverse = camera->view;
	closest = NULL;
	cx = x;
	cy = y;
	float		direction[4];
	direction[0] = ray.direction[0];
	direction[1] = ray.direction[1];
	direction[2] = ray.direction[2];
	direction[3] = 0.0f;
	i = -1;
	ray.t = FLT_MAX;
	ray.closest_t = FLT_MAX;
	ray.origin = camera->origin;
	//float		min_hit = FLT_MAX;
	while (++i < minirt->objects->size)
	{
		object = ft_darray_get(minirt->objects, i);
		//vmatmul(camera->inverse_view, object->center, ray.object_center);
		subtract_vectors(camera->origin, object->center, ray.object_center, 3);
		if (!object->intersect(object, &ray))
			continue;
		scale_vector(ray.direction, ray.t, 3);
		// if (ray.direction[2] < 0.0f || ray.direction[2] > min_hit)
		// 	continue;
		ray.closest_t = ray.t;
		//subtract_vectors(camera->origin, ray.direction, ray.direction, 3);
		scale_vector(ray.direction, ray.t, 3);
		subtract_vectors(ray.direction, object->center, normal, 3);
		hit_angle = dot_product(normalize_vector(normal, 3), minirt->light->direction, 3) * 0.5f + 0.5f;
		//hit_angle = clamp(hit_angle, 0.0f, 1.0f);
		closest = object;
		cx = x;
		cy = y;
		ray.direction[0] = direction[0];
		ray.direction[1] = direction[1];
		ray.direction[2] = direction[2];
		ray.direction[3] = 0.0f;
	}
	if (closest)
	{
		//fprintf(stdout, "closest %s %f\n", closest->name, min_hit);
		hit_color[0] = hit_angle * minirt->light->brightness * minirt->light->color[0];
		hit_color[1] = hit_angle * minirt->light->brightness * minirt->light->color[1];
		hit_color[2] = hit_angle * minirt->light->brightness * minirt->light->color[2];
		hit_color[3] = 0.0f;//
		hit_color[0] *= closest->color[0];
		hit_color[1] *= closest->color[1];
		hit_color[2] *= closest->color[2];
		minirt_pixel_put(display, cx, cy, to_argb(hit_color));
	}
	return (0);
}

int	render(t_minirt *minirt)
{
	int			i;
	int			j;
	t_display	*display;

	i = -1;
	display = minirt->display;
	while (++i < minirt->display->width)
	{
		j = -1;
		if ((i % 2))
			continue;
		while (++j < minirt->display->height)
			if (!(j % 2))
				draw_pixel(minirt, display, minirt->camera, i, j);
	}
	mlx_put_image_to_window(minirt->mlx, minirt->window, minirt->display->img, 0, 0);
	return (0);
}
