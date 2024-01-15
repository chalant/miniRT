#include "minirt.h"

int hit_point(float center[3], float radius, float direction[3], float *hit)
{
    float	a;
    float	b;
    float	c;
    float	discriminant;
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

int	hit_sphere(void *shape, t_object *object, t_ray *ray)
{
	t_sphere	*sphere;
	float	a;
    float	b;
    float	c;
    float	discriminant;
	float	t0;
	float	t1;

	(void)ray;
	(void)object;
	a = dot_product(ray->direction, ray->direction);
	b = 2.0f * dot_product(object->center, ray->direction);
	c = dot_product(object->center, object->center) - sphere->radius * sphere->radius;
	sphere = (t_sphere *)shape;
	return (0);
}

//generates sphere points and stores them into the objects body.
int	generate_sphere_points(t_object *object)
{
	(void)object;
	return (0);
}

int create_sphere(t_object *object, float radius)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (-1);
	object->object_shape = sphere;
	sphere->radius = radius;
	object->intersect = hit_sphere;
	return (0);
}
