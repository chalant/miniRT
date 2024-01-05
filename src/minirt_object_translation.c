/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_object_translation.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/05 14:10:46 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_translate(t_matrix *matrix, float x, float y, float z)
{
	matrix->points[0][matrix->cols - 1] = x;
	matrix->points[1][matrix->cols - 1] = y;
	matrix->points[2][matrix->cols - 1] = z;
}

void	set_translate_transform(t_minirt *fdf, t_object *object,
		t_matrix *transform)
{
	inplace_matmul(transform, object->transforms, fdf->tmp);
}

void	set_inv_translate_transform(t_minirt *fdf, t_object *object,
		t_matrix *rev_transform)
{
	t_matrix	*target;

	target = object->rev_transforms;
	matmul(target, rev_transform, fdf->tmp);
	matrix_copy(fdf->tmp, target);
}

void	translate_object(t_minirt *fdf, t_object *object, t_matrix *trs,
		t_matrix *inv)
{
	float	cx;
	float	cy;
	float	cz;

	transform_object(fdf, object, trs);
	cx = get_point(object->center, 0, 0);
	cy = get_point(object->center, 1, 0);
	cz = get_point(object->center, 2, 0);
	set_translate(object->t_center, cx, cy, cz);
	set_translate(object->rev_t_center, -cx, -cy, -cz);
	set_translate_transform(fdf, object, trs);
	set_inv_translate_transform(fdf, object, inv);
}
