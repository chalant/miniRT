/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_window_hooks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/03 15:54:12 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_mlx(t_minirt *fdf)
{
	if (fdf->display->img)
		mlx_destroy_image(fdf->mlx, fdf->display->img);
	if (fdf->window)
		mlx_destroy_window(fdf->mlx, fdf->window);
}

int	update_view(t_minirt *minirt)
{
	(void)minirt;
	return (0);
}

int	close_program(t_minirt *fdf)
{
	//minirt_cleanup(fdf, 0);
	free_mlx(fdf);
	exit(0);
}

int	handle_expose(t_minirt *fdf)
{
	(void)fdf;
	//update_view(fdf);
	return (0);
}
