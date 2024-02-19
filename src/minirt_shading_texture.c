/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_shading_texture.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:34:44 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 17:34:45 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	add_textures(t_minirt *minirt, t_hit *hit, float color[4])
{
	float			uv_coords[2];
	float			perturbation[3];
	t_perturbator	*perturbator;
	t_texture		*texture;

	hit->material = ft_darray_get(&minirt->materials,
			hit->object->material_index);
	perturbator = ft_darray_get(&minirt->perturbators,
			hit->object->perturbator_index);
	hit->object->uv_coords(hit->object, hit, uv_coords);
	perturbator->perturb_normal(perturbator, hit->object,
		uv_coords, perturbation);
	add_vectors(hit->normal, perturbation, hit->normal, 3);
	normalize_vector(hit->normal, hit->normal, 3);
	texture = ft_darray_get(&minirt->textures, hit->object->texture_index);
	texture->get_texture(hit->material, hit->object, uv_coords, color);
	return (0);
}
