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

int	minirt_init(t_minirt *fdf)
{
	fdf->rendered = 0;
	fdf->mlx = NULL;
	fdf->window = NULL;
	fdf->rotations = NULL;
	fdf->translations = NULL;
	fdf->rev_rotations = NULL;
	fdf->rev_translations = NULL;
	fdf->scalings = NULL;
	fdf->rev_scalings = NULL;
	fdf->transforms = NULL;
	fdf->centering = NULL;
	fdf->tmp = NULL;
	fdf->selected_object = NULL;
	fdf->display = NULL;
	fdf->objects = malloc(sizeof(t_darray));
	fdf->mouse.x = 0;
	fdf->mouse.y = 0;
	homogeneous_matrix(&fdf->mouse.direction, 3, 3);
	if (!fdf->objects)
		return (-1);
	if (ft_darray_init(fdf->objects, sizeof(t_object), 10) < 0)
		return (-1);
	fdf->camera = NULL;
	return (1);
}
