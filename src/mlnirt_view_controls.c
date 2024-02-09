/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlnirt_view_controls.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:07:04 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	zooming_hook(int code, t_minirt *fdf)
{
	if (code == ZI)
		inplace_matmul(fdf->zooming_in, fdf->transforms, fdf->tmp);
	else if (code == ZO)
		inplace_matmul(fdf->zooming_out, fdf->transforms, fdf->tmp);
	return (0);
}

int	view_translation_hook(int code, t_minirt *fdf)
{
	if (code == TU)
		inplace_matmul(fdf->rev_translations->x_axis,
			fdf->transforms, fdf->tmp);
	else if (code == TD)
		inplace_matmul(fdf->translations->x_axis, fdf->transforms, fdf->tmp);
	else if (code == TL)
		inplace_matmul(fdf->rev_translations->y_axis,
			fdf->transforms, fdf->tmp);
	else if (code == TR)
		inplace_matmul(fdf->translations->y_axis,
			fdf->transforms, fdf->tmp);
	return (0);
}
