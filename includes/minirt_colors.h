/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_rendering.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:54:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/29 15:54:53 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_COLORS_H
# define MINIRT_COLORS_H

# define BASE_COLOR 0xFFFFFF
# define MAX_COLOR

typedef struct s_hsl
{
	float			hue;
	float			saturation;
	float			lightness;
}					t_hsl;

unsigned int		hsl_to_rgb(t_hsl *hsl);
unsigned int		fdf_gradient(float fraction, unsigned int start,
						unsigned int end);
float				fdf_color(char *point_values);

t_hsl				rgb_to_hsl(float rr, float gg, float bb);

#endif
