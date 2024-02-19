/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_textures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:04:45 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:04:46 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*checkerboard(t_material *material, t_object *object,
	float uv_coords[2], float color[4])
{
	if (((int)(roundf(uv_coords[0] * material->repeat_pattern)) % 2 == 0)
		^ ((int)(roundf(uv_coords[1] * material->repeat_pattern)) % 2 == 0))
		return (copy_vector(object->color, color, 3));
	else
		return (copy_vector(material->color, color, 3));
	return (color);
}

float	*vertical_bands(t_material *material, t_object *object,
	float uv_coords[2], float color[4])
{
	if ((int)(roundf(uv_coords[0] * material->repeat_pattern)) % 2 == 0)
		return (copy_vector(object->color, color, 3));
	else
		return (copy_vector(material->color, color, 3));
	return (color);
}

float	*horizontal_bands(t_material *material, t_object *object,
	float uv_coords[2], float color[4])
{
	if ((int)(roundf(uv_coords[1] * material->repeat_pattern)) % 2 == 0)
		return (copy_vector(object->color, color, 3));
	else
		return (copy_vector(material->color, color, 3));
	return (color);
}

float	*no_texture(t_material *material, t_object *object,
	float uv_coords[2], float color[4])
{
	(void)material;
	(void)uv_coords;
	copy_vector(object->color, color, 3);
	return (color);
}
