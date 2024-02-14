/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_colors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:06:04 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*to_color(int rgb, float color[4])
{
	color[3] = (float)((rgb & (0xFF << 24)) >> 24) / 255.0f;
	color[0] = (float)((rgb & (0xFF << 16)) >> 16) / 255.0f;
	color[1] = (float)((rgb & (0xFF << 8)) >> 8) / 255.0f;
	color[2] = (float)((rgb & (0xFF))) / 255.0f;
	return (color);
}

float	*normalize_rgb(int rgb[3], float color[4])
{
	color[3] = 0.0f;
	color[0] = (float)(rgb[0]) / 255.0f;
	color[1] = (float)(rgb[1]) / 255.0f;
	color[2] = (float)(rgb[2]) / 255.0f;
	return (color);
}

float	clamp(float value, float min, float max)
{
	return (fmaxf(min, fminf(value, max)));
}

unsigned int	to_argb(float color[4])
{
	unsigned int	rgb;

	rgb = 0;
	rgb |= (unsigned int)(clamp(color[3], 0.0f, color[3]) * 255.0f) << 24;
	rgb |= (unsigned int)(clamp(color[0], 0.0f, 1.0f) * 255.0f) << 16;
	rgb |= (unsigned int)(clamp(color[1], 0.0f, 1.0f) * 255.0f) << 8;
	rgb |= (unsigned int)(clamp(color[2], 0.0f, 1.0f) * 255.0f);
	return (rgb);
}
