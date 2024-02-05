#include "minirt.h"

void    translate(t_minirt *minirt, t_object *object, t_matrix *axis)
{
    vmatmul(axis, object->center, axis);
}

void    rotate(t_minirt *minirt, t_object *object, t_matrix *axis)
{
    float   tmp[3];

    vmatmul(axis, object->orientation, tmp);
    object->orientation[0] = tmp[0];
    object->orientation[1] = tmp[1];
    object->orientation[2] = tmp[2];
}

void    scale(t_minirt *minirt, t_object *object, int axis)
{
    object->size[axis] *= axis;
}