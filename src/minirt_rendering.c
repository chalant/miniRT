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

    if (discriminant < 0) {
        // No intersection
        return 0;
    }

    // Compute the solutions for t
    float sqrt_discriminant = sqrt(discriminant);
   	t0 = (-b - sqrt_discriminant) / (2 * a);
    t1 = (-b + sqrt_discriminant) / (2 * a);

	hit[0] = t0;
	hit[1] = t1;
	// if (*hit <= 0)
	// 	return (0);
    // You can return the number of intersections, or the specific points based on your requirements.
    return 2;
}

// float *perspective_divide(float *vector)
// {
// 	if ()
// }

//todo: perform ray intersection with all objects in the scene.
//todo: 
int	draw_pixel(t_minirt *minirt, t_display *display, t_camera *camera, float *pixel)
{
	float	sphere_center[4];
	float	result[4];

	//clipping.
	// if (camera->transform->points[2][3] <= -1.0f || camera->transform->points[2][3] >= 1.0f)
	// 	return (0);
	camera->ray_direction[0] = 0.0f;
	camera->ray_direction[1] = 0.0f;
	camera->ray_direction[2] = 0.0f;
	camera->ray_direction[3] = 1.0f;
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	sphere_center[0] = 0.0f;
	sphere_center[1] = 0.0f;
	sphere_center[2] = -1.0f;
	sphere_center[3] = 1.0f;
	//ft_memset(result, 0, sizeof(float) * 3);
	//move the pixel to the camera coordinates system.
	vmatmul(minirt->world_space, pixel, result);
	//fprintf(stderr, "result: %f, %f, %f\n", result[0], result[1], result[3]);
	scale_vector(result, 1/result[3]);
	//fprintf(stderr, "result: %f, %f, %f\n", result[0], result[1], result[3]);
	unit_vector(result);
	result[3] = 0.0f;
	vmatmul(camera->inverse_transform, result, camera->ray_direction);
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	//vmatmul(camera->inverse_transform, sphere_center, result);
	//vmatmul(minirt->world_space, sphere_center, result);
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	float hit[2];
	float	camera_center[4];
	camera_center[0] = camera->transform->points[0][3];
	camera_center[1] = camera->transform->points[1][3];
	camera_center[2] = camera->transform->points[2][3];
	camera_center[3] = camera->transform->points[3][3];
	//fprintf(stderr, "camera_center: %f, %f, %f\n", camera_center[0], camera_center[1], camera_center[2]);
	vmatmul(camera->inverse_transform, camera_center, result);
	camera_center[0] = 0;
	camera_center[1] = 0;
	camera_center[2] = 0;
	camera_center[3] = 0;
	vmatmul(minirt->screen_space, result, camera_center);
	minirt_pixel_put(display, camera_center[0], camera_center[1], 0xff0000);
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	pixel[0] = 0.0f;
	pixel[1] = 0.0f;
	pixel[2] = 0.0f;
	pixel[3] = 0.0f;
	if (hit_sphere(sphere_center, 0.5f, camera->ray_direction, hit) == 2)
	{
		//fprintf(stderr, "HIT! %f %f\n", pixel[0], pixel[1]);
		//vmatmul(camera->transform, camera->ray_direction, pixel);
		// vmatmul(camera->inverse_transform, camera->ray_direction, result);
		//fprintf(stderr, "hit: %f, %f\n", hit[0], hit[1]);
		vmatmul(camera->transform, scale_vector(camera->ray_direction, hit[0]), result);
		//fprintf(stderr, "hit: %f, %f, %f\n", result[0], result[1], result[2]);
		// vmatmul(camera->inverse_transform, result, pixel);
		if (result[2] <= 0)
			return (0);
		scale_vector(result, 1 / result[2]);
		//fprintf(stderr, "result: %f, %f, %f\n", result[0], result[1], result[2]);
		// result[2] = 0.0f;
		vmatmul(minirt->screen_space, result, pixel);
		//fprintf(stderr, "pixel: %f, %f\n", pixel[0] + display->width / 2, pixel[1]);
		minirt_pixel_put(display, pixel[0], pixel[1], 255);
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
