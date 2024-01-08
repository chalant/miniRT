#include "minirt.h"

int	sphere_intersection(void *shape, t_object *object, t_ray *ray)
{
	//t_sphere	*sphere;

	(void)shape;
	(void)ray;
	(void)object;
	//sphere = (t_sphere *)shape;
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
	object->intersect = sphere_intersection;
	return (0);
}
