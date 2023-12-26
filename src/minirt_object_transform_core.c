/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_object_transform_core.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/29 15:53:50 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	transform_object(t_minirt *minirt, t_object *object, t_matrix *transform)
{
	(void)minirt;
	inplace_matmul(transform, object->body, object->result);
	inplace_matmul(transform, object->center, object->tmp);
}

void	apply_on_center(t_object *object, t_matrix *transform)
{
	inplace_matmul(object->rev_t_center, object->body, object->result);
	inplace_matmul(transform, object->body, object->result);
	inplace_matmul(object->t_center, object->body, object->result);
}
