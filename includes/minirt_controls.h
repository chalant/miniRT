/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_controls.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:41 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 18:10:22 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_CONTROLS_H
# define MINIRT_CONTROLS_H

# include "minirt.h"

typedef struct s_mouse
{
	t_matrix	direction;
	float		world_position[4];
	float		point[4];
	int			left_click;
	float		x;
	float		y;
	float		hit_point;
}				t_mouse;

typedef struct s_key
{
	int	pressed;
}				t_key;

#endif