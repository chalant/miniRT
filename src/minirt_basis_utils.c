/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_basis_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:02:52 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 17:04:00 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*choose_arbitray_vector(float result[3], float axis[3])
{
	result[0] = 0.0f;
	result[1] = 1.0f;
	result[2] = 0.0f;
	if (fabsf(dot_product(axis, result, 3)) == 1.0f)
	{
		result[0] = 0.0f;
		result[1] = 0.0f;
		result[2] = 1.0f;
	}
	else if (fabsf(dot_product(axis, result, 3)) == 1.0f)
	{
		result[0] = 1.0f;
		result[1] = 0.0f;
		result[2] = 0.0f;
	}
	return (result);
}

void	create_basis(float orientation[3], float u[3], float v[3])
{
	float	arbitrary[3];

	choose_arbitray_vector(arbitrary, orientation);
	cross_product(arbitrary, orientation, u);
	cross_product(u, orientation, v);
}

float	project_point(t_ray *ray, t_object *object, float hit)
{
	float	projection[3];

	scale_vector(ray->direction, hit, projection, 3);
	add_vectors(ray->origin, projection, projection, 3);
	subtract_vectors(object->center, projection, projection, 3);
	return (dot_product(projection, object->orientation, 3));
}
