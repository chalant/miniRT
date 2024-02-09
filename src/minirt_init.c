/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/05 15:29:28 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	minirt_init(t_minirt *minirt)
{
	minirt->rendered = 0;
	minirt->mlx = NULL;
	minirt->window = NULL;
	minirt->rotations = NULL;
	minirt->translations = NULL;
	minirt->rev_rotations = NULL;
	minirt->rev_translations = NULL;
	minirt->scalings = NULL;
	minirt->rev_scalings = NULL;
	minirt->transforms = NULL;
	minirt->centering = NULL;
	minirt->selected_object = NULL;
	minirt->mouse.x = 0;
	minirt->mouse.y = 0;
	minirt->ctrl.pressed = 0;
	homogeneous_matrix(&minirt->mouse.direction, 3, 3);
	if (ft_darray_init(&minirt->objects, sizeof(t_object), 10) < 0)
		return (-1);
	if (ft_darray_init(&minirt->materials, sizeof(t_material), 10) < 0)
		return (-1);
	if (ft_darray_init(&minirt->spot_lights, sizeof(t_light), 10) < 0)
		return (-1);
	return (1);
}
