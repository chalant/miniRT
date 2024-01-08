/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_key_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/30 12:48:19 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	movement_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
}

int	color_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
	return (1);
}

void	fdf_control_key(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
}

int	key_press_hook(int code, t_minirt *fdf)
{
	t_matrix	*tmp;

	homogeneous_matrix(&tmp, 3, 3);
	if (code == RL)
	{
		inplace_matmul(fdf->rotations->y_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rev_rotations->y_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == RR)
	{
		inplace_matmul(fdf->rev_rotations->y_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rotations->y_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == RU)
	{
		inplace_matmul(fdf->rotations->z_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rev_rotations->z_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == RD)
	{
		inplace_matmul(fdf->rev_rotations->z_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rotations->z_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == TL)
	{
		inplace_matmul(fdf->translations->x_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rev_translations->x_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == TR)
	{
		inplace_matmul(fdf->rev_translations->x_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->translations->x_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == TU)
	{
		inplace_matmul(fdf->translations->y_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->rev_translations->y_axis, fdf->camera->inverse_transform, tmp);
	}
	else if (code == TD)
	{
		inplace_matmul(fdf->rev_translations->y_axis, fdf->camera->transform, tmp);
		inplace_matmul(fdf->translations->y_axis, fdf->camera->inverse_transform, tmp);
	}
	delete_matrix(tmp);
	return (0);
}

int	key_release_hook(int code, t_minirt *fdf)
{
	(void)code;
	(void)fdf;
	return (0);
}
