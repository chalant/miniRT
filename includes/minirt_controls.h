#ifndef MINIRT_CONTROLS_H
# define MINIRT_CONTROLS_H

typedef struct	s_mouse
{
	float	direction[2];
	float	x;
	float	y;
}	t_mouse;

typedef struct	s_key
{
	int	pressed;
}	t_key;

#endif