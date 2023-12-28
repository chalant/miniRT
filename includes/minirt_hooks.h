#include "minirt.h"

int		key_release_hook(int code, t_minirt *minirt);
int		key_press_hook(int code, t_minirt *minirt);
void	fdf_control_key(int code, t_minirt *fdf);
int		color_hook(int code, t_minirt *fdf);
void	movement_hook(int code, t_minirt *fdf);
