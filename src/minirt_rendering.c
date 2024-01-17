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
	pixel[2] = 0.0f;
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

//todo: perform ray intersection with all objects in the scene.
int	draw_pixel(t_minirt *minirt, t_display *display, t_camera *camera, int x ,int y)
{
	int			i;
	float		result[4];
	float		point[4];
	t_object	*object;
	t_ray		ray;
	float		max_hit;
	float		light_direction[4];
	float		hit_color[4];
	float		hit_angle;

	to_screen_space(minirt->display, point, x, y);
	vmatmul(minirt->world_space, point, result);
	scale_vector(result, -1 / result[3], 3);
	result[3] = 1.0f;
	vmatmul(camera->inverse_transform, result, point);
	vmatmul(camera->inverse_transform, minirt->light->direction, light_direction);
	vmatmul(camera->transform, point, ray.direction);
	i = -1;
	max_hit = FLT_MAX;
	//todo: fix issue where when we are inside the sphere, the sphere behind gets displayed...
	//maybe use normals ? if the normal is negative only render the current sphere ?
	while (++i < minirt->objects->size)
	{
		object = ft_darray_get(minirt->objects, i);
		vmatmul(camera->inverse_transform, object->center, ray.object_center);
		if (object->intersect(object, &ray))
		{
			scale_vector(ray.direction, ray.t, 3);
			if (ray.direction[2] < 0.0f || ray.direction[2] > max_hit)
				continue;
			max_hit = ray.direction[2];
			ray.direction[3] = 0.0f;
			//vmatmul(camera->inverse_transform, ray.direction, result);
			hit_angle = dot_product(light_direction, normalize_vector(ray.direction, 3), 3) * 0.5 + 0.5;
			hit_color[0] = hit_angle * object->color[0] * minirt->light->brightness * minirt->light->color[0];
			hit_color[1] = hit_angle * object->color[1] * minirt->light->brightness * minirt->light->color[1];
			hit_color[2] = hit_angle * object->color[2] * minirt->light->brightness * minirt->light->color[2];
			hit_color[3] = 0.0f;
			//fprintf(stderr, "hit_color %f\n", hit_angle);
			// scale_vector(result, 1 / result[3], 3);
			// result[0] = ((result[0] + 1.0f) * (float)display->width) / 2.0f;
			// result[1] = ((result[1] + 1.0f) * (float)display->height) / 2.0f;
			//fprintf(stderr, "Color %f %f\n", object->color[0], object->color[1]);
			result[3] = 0.0f; 
			minirt_pixel_put(display, x, y, to_argb(hit_color));
			//minirt_pixel_put(display, result[0], result[1], 255);
		}
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
		while (++j < minirt->display->height)
		{
			minirt_pixel_put(display, i, j, 0);
			draw_pixel(minirt, minirt->display, minirt->camera, i, j);
		}
	}
	mlx_put_image_to_window(minirt->mlx, minirt->window, minirt->display->img, 0, 0);
	return (0);
}
