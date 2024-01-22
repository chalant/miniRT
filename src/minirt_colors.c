/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_colors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/30 12:47:08 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

unsigned int	str_to_uint(char *str, int base)
{
	unsigned int	res;
	int				i;

	res = 0;
	i = 2;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			res = res * base + (str[i] - '0');
		else if (ft_isalpha(str[i]))
			res = res * base + (ft_toupper(str[i]) - 'A' + 10);
		i++;
	}
	return (res);
}

unsigned int	get_gradient(int start, int end, float fraction)
{
	return ((unsigned int)((1 - fraction) * start + fraction * end));
}

unsigned int	fdf_gradient(float fraction, unsigned int start,
		unsigned int end)
{
	int	r;
	int	g;
	int	b;

	if (start == end)
		return (start);
	r = get_gradient((start >> 16) & 0xFF, (end >> 16) & 0xFF, fraction);
	g = get_gradient((start >> 8) & 0xFF, (end >> 8) & 0xFF, fraction);
	b = get_gradient(start & 0xFF, end & 0xFF, fraction);
	return ((r << 16) | (g << 8) | b);
}

float	*to_color(int rgb, float color[4])
{
	color[3] = (float)((rgb & (0xFF << 24)) >> 24) / 255.0f;
	color[0] = (float)((rgb & (0xFF << 16)) >> 16) / 255.0f;
	color[1] = (float)((rgb & (0xFF << 8)) >> 8) / 255.0f;
	color[2] = (float)((rgb & (0xFF))) / 255.0f;
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
	rgb |= (unsigned int)(clamp(color[0], 0.0f, color[0]) * 255.0f) << 16;
	rgb |= (unsigned int)(clamp(color[1], 0.0f, color[1]) * 255.0f) << 8;
	rgb |= (unsigned int)(clamp(color[2], 0.0f, color[2]) * 255.0f);
	return (rgb);
}
