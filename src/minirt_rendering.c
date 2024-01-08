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

// //todo: handle errors
// int	set_viewport_transforms(t_camera *camera, t_display *display, float focal_length)
// {
// 	float		tmp[4];
// 	t_matrix	*transform;

// 	if (homogeneous_matrix(&transform, 3, 3) < 0)
// 		return (-1);
// 	tmp[0] = 0.5f;
// 	tmp[1] = 0.5f;
// 	tmp[3] = 1.0f;
// 	//todo: the origin must be homogeneous.
// 	camera->pixel_origin = ft_calloc(4, sizeof(float));
// 	camera->pixel_origin[3] = 1.0f;
// 	camera->viewport_u = camera->width / display->width; 
// 	camera->viewport_v = -camera->height / display->height;
// 	transform->points[0][0] = camera->viewport_u;
// 	transform->points[1][1] = camera->viewport_v;
// 	transform->points[2][0] = camera->origin[0] - camera->width / 2.0f;
// 	transform->points[2][1] = camera->origin[1] + camera->height / 2.0f;
// 	transform->points[2][2] = focal_length;
// 	vmatmul(transform, tmp, camera->pixel_origin);
// 	delete_matrix(transform);
// 	return (0);
// }

int	set_pixel(t_display *display, t_camera *camera, float *pixel, float i, float j)
{
	(void)camera;
	pixel[0] = (i / (float)display->width) * 2.0f - 1.0f;
	pixel[1] = (j / (float)display->height) * 2.0f - 1.0f;
	// pixel[0] = i;
	// pixel[1] = j;
	pixel[2] = 0.0f;
	pixel[3] = 1.0f;
	//fprintf(stderr, "pixel: %f, %f\n", camera->viewport_u, camera->viewport_v);
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

float	*unit_vector(float *vector, float scale)
{
	float	magnitude;

	magnitude = sqrtf(vector[0] * vector[0] + vector[1] * vector[1]
			+ vector[2] * vector[2]);
	vector[0] *= scale;
	vector[1] *= scale;
	vector[2] *= scale;
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

int	hit_sphere(float center[3], float radius, float origin[3], float direction[3])
{
	float	a;
	float	b;
	float	c;
	float	discriminant;

	(void)origin;
	a = dot_product(direction, direction);
	b = 2.0f * dot_product(center, direction);
	c = dot_product(center, center) - radius * radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1);
	return (1);
}

//todo: perform ray intersection with all objects in the scene.
//todo: 
int	draw_pixel(t_minirt *minirt, t_display *display, t_camera *camera, float *pixel)
{
	float	sphere_center[3];
	float	result[4];

	camera->ray_direction[0] = 0.0f;
	camera->ray_direction[1] = 0.0f;
	camera->ray_direction[2] = 0.0f;
	camera->ray_direction[3] = 1.0f;
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 1.0f;
	sphere_center[0] = 0.0f;
	sphere_center[1] = 0.0f;
	sphere_center[2] = -2.0f;
	//ft_memset(result, 0, sizeof(float) * 3);
	//move the pixel to the camera coordinates system.
	vmatmul(minirt->world_space, pixel, result);
	//fprintf(stderr, "result: %f, %f, %f\n", result[0], result[1], result[3]);
	vmatmul(camera->transform, result, camera->ray_direction);
	vmatmul(camera->transform, camera->origin, result);
	// fdf->camera->origin[0] = vector[0];
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 1.0f;
	camera->origin[0] = result[0];
	camera->origin[1] = result[1];
	camera->origin[2] = result[2];
	camera->origin[3] = result[3];
	//unit_vector(camera->ray_direction, 800.0);
	//todo: perform ray intersection with all objects in the scene.
	//todo: move back to screen coordinates.
	// fprintf(stderr, "ray direction: %f, %f, %f\n", camera->ray_direction[0], camera->ray_direction[1], camera->ray_direction[2]);
	//vmatmul(camera->transform, sphere_center, result);
	pixel[0] = 0.0f;
	pixel[1] = 0.0f;
	pixel[2] = 0.0f;
	pixel[3] = 1.0f;
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 1.0f;
	if (hit_sphere(sphere_center, 0.5f, camera->origin, camera->ray_direction) == 1)
	{
		//fprintf(stderr, "HIT\n");
		//vmatmul(camera->transform, camera->ray_direction, pixel);
		vmatmul(camera->inverse_transform, camera->ray_direction, result);
		vmatmul(minirt->screen_space, result, pixel);
		result[0] = 0.0f;
		result[1] = 0.0f;
		result[2] = 0.0f;
		result[3] = 1.0f;
		vmatmul(camera->inverse_transform, camera->origin, result);
		camera->origin[0] = result[0];
		camera->origin[1] = result[1];
		camera->origin[2] = result[2];
		camera->origin[3] = result[3];
		//fprintf(stderr, "pixel: %f, %f\n", pixel[0] + display->width / 2, pixel[1]);
		minirt_pixel_put(display, pixel[0] - display->width, pixel[1] - display->height, 255);
	}
	// pixel[0] = 0.0f;
	// pixel[1] = 0.0f;
	// pixel[2] = 0.0f;
	// pixel[3] = 1.0f;
	//vmatmul(camera->transform, camera->ray_direction, pixel);
	// vmatmul(camera->inverse_transform, camera->origin, pixel);
	// camera->origin[0] = pixel[0];
	// camera->origin[1] = pixel[1];
	// camera->origin[2] = pixel[2];
	//minirt_pixel_put(display, camera->origin[1], camera->origin[0], 0xFF0000);
	//vmatmul(minirt->screen_space, camera->ray_direction, pixel);
	//todo: move back to screen coordinates.
	//minirt_pixel_put(display, pixel[0], pixel[1], 255);
	//fprintf(stderr, "pixel: %f, %f\n", pixel[0], pixel[1]);
	//}
	//fprintf(stderr, "pixel: %f, %f\n", pixel[0], pixel[1]);
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
	ft_memset(display->addr, 0, display->width * display->height
		* (display->bits_per_pixel / 8));
	//TODO: should transform all points in the world to the camera coordinates system.
	//uset the camera orientation to create the transform matrix.
	//rays will be cast from the camera into the viewport.
	//todo: need a change of basis where we use angles of the camera orientation with respect
	// to the world coordinates. 
	// todo: need to move to pixel space.
	minirt->camera->height = 2.0f;
	minirt->camera->width = minirt->camera->height * (float)(minirt->display->width / minirt->display->height);
	//set_viewport_transforms(minirt->camera, minirt->display, 1.0f);
	// print_matrix(minirt->camera->transform);
	// printf("\n");
	// print_matrix(minirt->camera->inverse_transform);
	while (++i < minirt->display->width)
	{
		j = -1;
		while (++j < minirt->display->height)
		{
			//reset pixel
			//minirt_pixel_put(minirt->display, i, j, 0);
			set_pixel(minirt->display, minirt->camera, pixel, i, j);
			draw_pixel(minirt, minirt->display, minirt->camera, pixel);
			// while (++k < minirt->objects->size)
			// {

			// 	obj = (t_object *)minirt->objects->contents + k;
			// }
		}
	}
	mlx_put_image_to_window(minirt->mlx, minirt->window, minirt->display->img, 0, 0);
	return (0);
}
