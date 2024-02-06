#include "minirt.h"
#include "float.h"

int	hit_sphere(t_object *object, t_ray *ray)
{
	float		a;
    float		b;
    float		c;
    float		discriminant;
	float		t;
	float		center[3];

	subtract_vectors(ray->origin, object->center, center, 3);
	a = dot_product(ray->direction, ray->direction, 3);
	b = 2.0f * dot_product(center, ray->direction, 3);
	c = dot_product(center, center, 3) - object->size[0] * object->size[0];
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

	subtract_vectors(hit->ray_origin, hit->object->center, center, 3);
	add_vectors(hit->point, center, hit->normal, 3);
	scale_vector(hit->normal, 1 / object->size[0], hit->normal, 3);
	return (hit->normal);
}

float	*sphere_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
{
	float		theta;
	float		phi;

	phi = atan2f(hit->point[2] - object->center[2], hit->point[0] - object->center[0]);
	theta = acosf((hit->point[1] - object->center[1]) / object->size[0]);
	uv_coords[0] = (phi + M_PI) / (2.0f * M_PI);
    uv_coords[1] = 1.0f - theta / M_PI;
	return (uv_coords);
}

float	*plane_uv_coords(t_object *object, t_hit *hit, float uv_coords[2])
{
	float		*normal;

	normal = object->orientation;
	if (fabsf(normal[0]) > fabsf(normal[1]) && fabsf(normal[0]) > fabsf(normal[2]))
	{
		uv_coords[0] = hit->point[1] - object->center[1];
		uv_coords[1] = hit->point[2] - object->center[2];
	}
	else if (fabsf(normal[1]) > fabsf(normal[0]) && fabsf(normal[1]) > fabsf(normal[2]))
	{
		uv_coords[0] = -hit->point[0] + object->center[0];
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
	(void)name;
	object->intersect = hit_sphere;
	object->normal = sphere_normal;
	object->uv_coords = sphere_uv_coords;
	object->size[0] = radius;
	object->size[1] = 0.0f;
	object->size[2] = 0.0f;
	return (0);
}

int	hit_plane(t_object *object, t_ray *ray)
{
	float		denominator;
	float		numerator;
	float		t;
	float		result[3];

	denominator = -dot_product(object->orientation, ray->direction, 3);
	if (fabsf(denominator) < 0.0001f)
		return (0);
	subtract_vectors(ray->origin, object->center, result, 3);
	numerator = dot_product(object->orientation, result, 3);
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
	hit->normal[0] = object->orientation[0];
	hit->normal[1] = object->orientation[1];
	hit->normal[2] = object->orientation[2];
	return (hit->normal);
}

int	create_plane(t_object *object, float normal[4])
{
	object->orientation[0] = normal[0];
	object->orientation[1] = normal[1];
	object->orientation[2] = normal[2];
	normalize_vector(object->orientation, object->orientation, 3);
	object->intersect = hit_plane;
	object->normal = plane_normal;
	object->uv_coords = plane_uv_coords;
	return (0);
}
