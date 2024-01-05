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
	fdf->zooming_in = NULL;
	fdf->zooming_out = NULL;
	fdf->target = NULL;
	fdf->display = NULL;
	fdf->objects = malloc(sizeof(t_darray));
	if (!fdf->objects)
		return (-1);
	if (ft_darray_init(fdf->objects, sizeof(t_object), 10) < 0)
		return (-1);
	fdf->camera = NULL;
	return (1);
}
