#include "minirt.h"

int	do_nothing(t_object *object, t_matrix *transform)
{
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

	sphere = (t_sphere *)object->shape;
	a = dot_product(ray->direction, ray->direction, 3);
	b = 2.0f * dot_product(ray->object_center, ray->direction, 3);
	c = dot_product(ray->object_center, ray->object_center, 3) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f)
		return (0);
	ray->t = (-b - sqrt(discriminant)) / (2 * a);
	return (1);
}

int create_sphere(t_object *object, float radius)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
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
	//static int	count;

	plane = (t_plane *)object->shape;
	vmatmul(ray->transform, plane->normal, tmp);
	denominator = dot_product(tmp, ray->direction, 3);
	//fprintf(stderr, "center %f %f %f\n", ray->object_center[0], ray->object_center[1], ray->object_center[2]);
	// if (!(count % 100000))
	// 	fprintf(stderr, "tmp %f %f %f\n", tmp[0], tmp[1], tmp[2]);
	// count++;
	if (fabsf(denominator) < 0.0001f)
		return (0);
	numerator = dot_product(tmp, ray->object_center, 3);
	//fprintf(stderr, "denominator %f\n", denominator);
	t = (numerator / denominator);
	//fprintf(stderr, "numerator %f\n", numerator);
	if (t < 0.0f)
		return (0);
	//fprintf(stderr, "t %f\n", t);
	ray->t = -t;
	return (1);
}

int	transform_plane(t_object *object, t_matrix *transform)
{
	t_plane		*plane;
	float		tmp[4];

	plane = (t_plane *)object->shape;
	vmatmul(transform,  plane->normal, tmp);
	fprintf(stderr, "normal %f %f %f\n", tmp[0], tmp[1], tmp[2]);
	plane->normal[0] = tmp[0];
	plane->normal[1] = tmp[1];
	plane->normal[2] = tmp[2];
	plane->normal[3] = 1.0f;
	vmatmul(transform, object->center, tmp);
	object->center[0] = tmp[0];
	object->center[1] = tmp[1];
	object->center[2] = tmp[2];
	object->center[3] = 1.0f;
	fprintf(stderr, "center %f %f %f\n", object->center[0], object->center[1], object->center[2]);
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
	plane->normal[3] = normal[3];
	object->shape = plane;
	object->intersect = hit_plane;
	object->transform = transform_plane;
	return (0);
}
