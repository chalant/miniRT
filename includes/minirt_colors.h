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

unsigned int		to_argb(float color[4]);
float				*to_color(int rgb, float color[4]);
float				clamp(float value, float min, float max);
float				*normalize_rgb(int rgb[3], float color[4]);

#endif
