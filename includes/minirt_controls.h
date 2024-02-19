#ifndef MINIRT_CONTROLS_H
# define MINIRT_CONTROLS_H

#include "minirt.h"

typedef struct	s_mouse
{
	t_matrix	direction;
	float		world_position[4];
	float		point[4];
	int			left_click;
	float		x;
	float		y;
	float		hit_point;
}				t_mouse;

typedef struct	s_key
{
	int	pressed;
}				t_key;

#endif