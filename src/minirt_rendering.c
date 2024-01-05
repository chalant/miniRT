#include "minirt.h"

void	minirt_pixel_put(t_display *display, int x, int y, int color)
{
	char	*dst;

	if (x <= 0 || y <= 0)
		return ;
	if (x >= display->width || y >= display->height)
		return ;
	dst = display->addr + (y * display->line_length + x
			* (display->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	render(t_minirt *minirt)
{
	int			i;
	int			j;
	int			k;
	t_object	*obj;

	i = -1;
	// ft_memset(display->addr, 0x0, display->width * display->height
	// 	* (display->bits_per_pixel / 8));
	//TODO: should transform all points in the world to the camera coordinates system.
	//uset the camera orientation to create the transform matrix.
	//rays will be cast from the camera into the viewport.
	//todo: need a change of basis where we use angles of the camera orientation with respect
	// to the world coordinates. 
	// todo: need to move to pixel space.
	while (++i < minirt->camera->width)
	{
		j = -1;
		while (++j < minirt->camera->height)
			while (++k < minirt->objects->size)
				obj = (t_object *)minirt->objects->contents + i;
	}
	return (0);
}