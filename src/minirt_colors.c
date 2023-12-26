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

float	fdf_color(char *point_values)
{
	unsigned int	rgb;
	char			**color;
	t_hsl			hsl;

	if (!ft_strchr(point_values, ','))
		return (0.0f);
	color = ft_split(point_values, ',');
	if (!color)
		return (0);
	rgb = str_to_uint(color[1], 16);
	hsl = rgb_to_hsl((float)((rgb >> 16) & 0xFF),
			(float)((rgb >> 8) & 0xFF), (float)(rgb & 0xFF));
	delete_strings(color);
	return (hsl.hue);
}
