/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_rendering.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:00:12 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 17:35:47 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	minirt_pixel_put(t_display *display, int x, int y, int color)
{
	char	*dst;

	if (x <= 0 || y <= 0)
		return ;
	if (x >= display->width || y >= display->height)
		return ;
	dst = display->addr + (y * display->line_length + x
			* display->offset);
	*(unsigned int *)dst = color;
}

int	low_resolution(t_minirt *minirt)
{
	int	coords[2];
	int	skip_x;

	coords[0] = -1;
	while (++coords[0] < minirt->display.width)
	{
		coords[1] = -1;
		skip_x = coords[0] % 4;
		while (++coords[1] < minirt->display.height)
		{
			minirt_pixel_put(&minirt->display, coords[0],
				coords[1], 0x00000000);
			if (skip_x || (coords[1] % 4))
				continue ;
			shade_pixel(minirt, coords);
		}
	}
	return (0);
}

int	full_resolution(t_minirt *minirt)
{
	int	coords[2];

	coords[0] = -1;
	while (++coords[0] < minirt->display.width)
	{
		coords[1] = -1;
		while (++coords[1] < minirt->display.height)
			shade_pixel(minirt, coords);
	}
	return (0);
}

int	render(t_minirt *minirt)
{
	minirt->render_mode(minirt);
	mlx_put_image_to_window(minirt->mlx, minirt->window,
		minirt->display.img, 0, 0);
	return (0);
}
