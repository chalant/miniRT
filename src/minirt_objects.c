#include "minirt.h"

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
	return (0);
}
