/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_window_hooks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:47:23 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_mlx(t_minirt *minirt)
{
	if (minirt->display.img)
		mlx_destroy_image(minirt->mlx, minirt->display.img);
	if (minirt->window)
		mlx_destroy_window(minirt->mlx, minirt->window);
}

int	close_program(t_minirt *minirt)
{
	minirt_cleanup(minirt);
	free_mlx(minirt);
	exit(0);
}
