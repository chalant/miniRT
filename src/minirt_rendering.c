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

int	set_pixel(t_display *display, float *pixel, float i, float j)
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

float	*unit_vector(float *vector)
{
	float	magnitude;

	magnitude = sqrtf(vector[0] * vector[0] + vector[1] * vector[1]
			+ vector[2] * vector[2]);
	vector[0] /= magnitude;
	vector[1] /= magnitude;
	vector[2] /= magnitude;
	return (vector);
}

float	*scale_vector(float *vector, float scale)
{
	vector[0] *= scale;
	vector[1] *= scale;
	vector[2] *= scale;
	return (vector);
}

float	dot_product(float v1[3], float v2[3])
{
	return (v1[0] * v2[0] + v1[1] * v2[1]
			+ v1[2] * v2[2]);
}

int hit_sphere(float center[3], float radius, float direction[3], float *hit)
{
    float a;
    float b;
    float c;
    float discriminant;
	float	t0;
	float	t1;

    a = dot_product(direction, direction);
    b = 2.0f * dot_product(center, direction);
    c = dot_product(center, center) - radius * radius;
    discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
        return (0);
    // Compute the solutions for t
    float sqrt_discriminant = sqrt(discriminant);
   	t0 = (-b - sqrt_discriminant) / (2 * a);
    t1 = (-b + sqrt_discriminant) / (2 * a);

	hit[0] = t0;
	hit[1] = t1;
    return (1);
}

//todo: perform ray intersection with all objects in the scene.
int	draw_pixel(t_minirt *minirt, t_display *display, t_camera *camera, float *pixel, int i , int j)
{
	float	sphere_center[4];
	float	result[4];
	float	world_point[4];
	static	int	count = 0;
	
	camera->ray_direction[0] = 0.0f;
	camera->ray_direction[1] = 0.0f;
	camera->ray_direction[2] = 0.0f;
	camera->ray_direction[3] = 0.0f;
	world_point[0] = 0.0f;
	world_point[1] = 0.0f;
	world_point[2] = 0.0f;
	world_point[3] = 0.0f;
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	sphere_center[0] = 0.0f;
	sphere_center[1] = 0.0f;
	sphere_center[2] = -1.0f;
	sphere_center[3] = 1.0f;

	vmatmul(minirt->world_space, pixel, result);
	scale_vector(result, -1/result[3]);
	unit_vector(result);
	result[3] = 1.0f;
	
	vmatmul(camera->inverse_transform, result, world_point);
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	vmatmul(camera->transform, world_point, camera->ray_direction);
	
	float	hit[2];
	count++;
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	pixel[0] = 0.0f;
	pixel[1] = 0.0f;
	pixel[2] = 0.0f;
	pixel[3] = 0.0f;
	vmatmul(camera->inverse_transform, sphere_center, result);
	sphere_center[0] = result[0];
	sphere_center[1] = result[1];
	sphere_center[2] = result[2];
	sphere_center[3] = result[3];
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	if (hit_sphere(sphere_center, 0.2f, camera->ray_direction, hit))
	{
		//vmatmul(camera->inverse_transform, scale_vector(camera->ray_direction, hit[0]), result);
		scale_vector(camera->ray_direction, hit[0]);
		if (camera->ray_direction[2] <= 0.0f)
			return (0);
		camera->ray_direction[3] = 1.0f;
		vmatmul(minirt->view_matrix, camera->ray_direction, result);
		scale_vector(result, 1 / result[3]);
		result[0] = ((result[0] + 1.0f) * display->width) / 2.0f;
		result[1] = ((result[1] + 1.0f) * display->height) / 2.0f;
		minirt_pixel_put(display, i, j, 0xf94449);
		//minirt_pixel_put(display, result[0], result[1], 255);
	}
	return (0);
}

int	render(t_minirt *minirt)
{
	int			i;
	int			j;
	float 		pixel[4];
	t_display	*display;

	i = -1;
	display = minirt->display;
	minirt->camera->height = 2.0f;
	minirt->camera->width = minirt->camera->height * (float)(minirt->display->width / minirt->display->height);
	while (++i < minirt->display->width)
	{
		j = -1;
		while (++j < minirt->display->height)
		{
			minirt_pixel_put(display, i, j, 0);
			set_pixel(minirt->display, pixel, i, j);
			draw_pixel(minirt, minirt->display, minirt->camera, pixel, i, j);
		}
	}
	mlx_put_image_to_window(minirt->mlx, minirt->window, minirt->display->img, 0, 0);
	return (0);
}
