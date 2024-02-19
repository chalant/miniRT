/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_material_edit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:13:01 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:13:02 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	update_pattern(t_material *material, float direction[4])
{
	material->repeat_pattern += direction[1];
	return (0);
}

int	update_material(t_minirt *minirt, t_object *object, float direction[4])
{
	t_material	*material;

	material = ft_darray_get(&minirt->materials, object->material_index);
	if (minirt->ctrl.pressed)
		return (update_pattern(material, direction));
	material->shininess += direction[1] * minirt->mouse.hit_point * 5.0f;
	if (material->shininess < 2.0f)
		material->shininess = 2.0f;
	else if (material->shininess > 200.0f)
		material->shininess = 200.0f;
	material->reflectivity += -direction[0];
	if (material->reflectivity >= 0.5f)
		material->reflectivity = 0.5f;
	else if (material->reflectivity < 0.0f)
		material->reflectivity = 0.0f;
	return (0);
}
