/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/03 14:02:00 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	minirt_init(t_minirt *fdf)
{
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
	return (1);
}
