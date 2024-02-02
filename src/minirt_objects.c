#include "minirt.h"
#include "float.h"

int	do_nothing(t_object *object, t_matrix *transform, float *result)
{
	(void)result;
	(void)object;
	(void)transform;
	return (0);
}

int	hit_sphere(t_object *object, t_ray *ray)
{
	t_sphere	*sphere;
	float		a;
    float		b;
    float		c;
    float		discriminant;
	float		t;
	float		center[3];

	sphere = (t_sphere *)object->shape;
	subtract_vectors(ray->origin, object->center, center, 3);
	a = dot_product(ray->direction, ray->direction, 3);
	b = 2.0f * dot_product(center, ray->direction, 3);
	c = dot_product(center, center, 3) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f)
		return (0);
	t = (-b - sqrt(discriminant)) / (2 * a);
	if (t < 0.0f || t > ray->closest_t)
		t = (-b + sqrt(discriminant)) / (2 * a);
	if (t < 0.0f)
		return (0);
	if (t > ray->closest_t)
		return (0);
	ray->t = t;
	return (1);
}

float	*sphere_normal(t_object *object, t_hit *hit)
{
	float		center[3];
	t_sphere	*sphere;

	sphere = object->shape;
	subtract_vectors(hit->ray_origin, hit->object->center, center, 3);
	add_vectors(hit->point, center, hit->normal, 3);
	scale_vector(hit->normal, 1 / sphere->radius, hit->normal, 3);
	//normalize_vector(hit->normal, hit->normal, 3);
	return (hit->normal);
}

float	*sphere_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
{
	t_sphere	*sphere;
	float		theta;
	float		phi;

	sphere = object->shape;
	phi = atan2f(hit->point[2] - object->center[2], hit->point[0] - object->center[0]);
	theta = acosf((hit->point[1] - object->center[1]) / sphere->radius);
	uv_coords[0] = (phi + M_PI) / (2.0f * M_PI);
	uv_coords[1] = 1.0f - (theta / M_PI);
	return (uv_coords);
}

float	*plane_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
{
	t_plane		*plane;

	plane = object->shape;
	if (fabsf(plane->normal[0]) > fabsf(plane->normal[1]) && fabsf(plane->normal[0]) > fabsf(plane->normal[2]))
	{
		uv_coords[0] = hit->point[1] - object->center[1];
		uv_coords[1] = hit->point[2] - object->center[2];
	}
	else if (fabsf(plane->normal[1]) > fabsf(plane->normal[0]) && fabsf(plane->normal[1]) > fabsf(plane->normal[2]))
	{
		uv_coords[0] = hit->point[0] - object->center[0];
		uv_coords[1] = hit->point[2] - object->center[2];
	}
	else
	{
		uv_coords[0] = hit->point[0] - object->center[0];
		uv_coords[1] = hit->point[1] - object->center[1];
	}
	return (uv_coords);
}

int create_sphere(t_object *object, float radius, const char *name)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	sphere->name = name;
	if (!sphere)
		return (-1);
	sphere->radius = radius;
	object->shape = sphere;
	object->intersect = hit_sphere;
	object->transform = do_nothing;
	object->normal = sphere_normal;
	object->uv_coords = sphere_uv_coords;
	return (0);
}

int	hit_plane(t_object *object, t_ray *ray)
{
	t_plane		*plane;
	float		denominator;
	float		numerator;
	float		t;
	float		result[3];

	plane = object->shape;
	denominator = -dot_product(plane->normal, ray->direction, 3);
	if (fabsf(denominator) < 0.0001f)
		return (0);
	subtract_vectors(ray->origin, object->center, result, 3);
	numerator = dot_product(plane->normal, result, 3);
	t = (numerator / denominator);
	if (t < 0.0f)
		return (0);
	if (t > ray->closest_t)
		return (0);
	ray->t = t;
	return (1);
}

float	*plane_normal(t_object *object, t_hit *hit)
{
	t_plane	*plane;

	plane = object->shape;
	hit->normal[0] = plane->normal[0];
	hit->normal[1] = plane->normal[1];
	hit->normal[2] = plane->normal[2];
	return (hit->normal);
}

int	transform_plane(t_object *object, t_matrix *transform, float *result)
{
	t_plane		*plane;

	plane = object->shape;
	vmatmul(transform,  plane->normal, result);
	return (0);
}

int	create_plane(t_object *object, float normal[4])
{
	t_plane	*plane;

	plane = malloc(sizeof(t_plane));
	if (!plane)
		return (-1);
	plane->normal[0] = normal[0];
	plane->normal[1] = normal[1];
	plane->normal[2] = normal[2];
	plane->normal[3] = 0.0f;
	normalize_vector(plane->normal, plane->normal, 3);
	object->shape = plane;
	object->intersect = hit_plane;
	object->transform = transform_plane;
	object->normal = plane_normal;
	object->uv_coords = plane_uv_coords;
	return (0);
}
