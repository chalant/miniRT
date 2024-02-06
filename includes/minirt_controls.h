#ifndef MINIRT_CONTROLS_H
# define MINIRT_CONTROLS_H

#include "minirt.h"

typedef struct	s_mouse
{
	t_matrix	*direction;
	int			capture;
	int			right_click;
	int			left_click;
	float		x;
	float		y;
}				t_mouse;

typedef struct	s_key
{
	int	pressed;
}				t_key;

typedef	struct	s_controls
{

}				t_controls;

#endif