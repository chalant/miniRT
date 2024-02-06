#include "minirt.h"

void	translate(t_minirt *minirt, t_object *object, int axis, float speed)
{
	(void)minirt;
	object->center[axis] += speed;
}

void	rotate(t_minirt *minirt, t_object *object, t_matrix *axis)
{
	float   tmp[3];

	(void)minirt;
	vmatmul(axis, object->orientation, tmp);
	object->orientation[0] = tmp[0];
	object->orientation[1] = tmp[1];
	object->orientation[2] = tmp[2];
}

void	scale(t_minirt *minirt, t_object *object, int axis, float rate)
{
	(void)minirt;
	(void)object;
	object->size[axis] *= rate;
}
