#ifndef MINIRT_CONTROLS_H
# define MINIRT_CONTROLS_H

typedef struct	s_mouse
{
	t_matrix	*direction;
	int			capture;
	float		x;
	float		y;
}	t_mouse;

typedef struct	s_key
{
	int	pressed;
}	t_key;

#endif