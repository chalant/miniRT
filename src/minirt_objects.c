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

	sphere = (t_sphere *)object->shape;
	a = dot_product(ray->direction, ray->direction, 3);
	b = 2.0f * dot_product(ray->object_center, ray->direction, 3);
	c = dot_product(ray->object_center, ray->object_center, 3) - sphere->radius * sphere->radius;
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
	return (0);
}

int	hit_plane(t_object *object, t_ray *ray)
{
	t_plane		*plane;
	float		denominator;
	float		numerator;
	float		t;
	float		tmp[4];
	float		tmp2[4];

	plane = (t_plane *)object->shape;
	vmatmul(ray->transform, plane->normal, tmp);
	vmatmul(ray->inverse, plane->normal, tmp2);
	normalize_vector(tmp, 3);
	normalize_vector(tmp2, 3);
	denominator = dot_product(tmp2, ray->direction, 3);
	if (fabsf(denominator) < 0.0001f)
		return (0);
	numerator = dot_product(tmp, ray->object_center, 3);
	t = (numerator / denominator);
	if (t < 0.0f)
		return (0);
	//fprintf(stderr, "t %f\n", t);
	if (t > ray->closest_t)
		return (0);
	ray->t = t;
	// delete_matrix(matrix);
	return (1);
}

int	transform_plane(t_object *object, t_matrix *transform, float *result)
{
	t_plane		*plane;

	plane = (t_plane *)object->shape;
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
	plane->normal[2] = -normal[2];
	plane->normal[3] = 0.0f;
	normalize_vector(plane->normal, 3);
	object->shape = plane;
	object->intersect = hit_plane;
	object->transform = transform_plane;
	return (0);
}
