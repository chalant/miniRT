/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_object_rotation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/29 15:53:50 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_rotate_transforms(t_minirt *fdf, t_object *object, t_matrix *transform)
{
	t_matrix	*target;

	target = object->transforms;
	inplace_matmul(object->rev_t_center, target, fdf->tmp, 3);
	inplace_matmul(transform, target, fdf->tmp, 3);
	inplace_matmul(object->t_center, target, fdf->tmp, 3);
}

void	set_inverse_rotate_transforms(t_minirt *fdf, t_object *object,
		t_matrix *rev_transform)
{
	t_matrix	*target;

	target = object->rev_transforms;
	matmul(target, object->t_center, fdf->tmp, 3);
	matrix_copy(fdf->tmp, target);
	matmul(target, rev_transform, fdf->tmp, 3);
	matrix_copy(fdf->tmp, target);
	matmul(target, object->rev_t_center, fdf->tmp, 3);
	matrix_copy(fdf->tmp, target);
}

void	rotate_object(t_minirt *fdf, t_object *object, t_matrix *transform,
		t_matrix *rev_transform)
{
	apply_on_center(object, transform);
	set_rotate_transforms(fdf, object, transform);
	set_inverse_rotate_transforms(fdf, object, rev_transform);
}
