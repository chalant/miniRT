#ifndef MINIRT_H
# define MINIRT_H

# include <stdlib.h>
# include <math.h>
# include "matrix.h"
# include "minirt_bindings.h"
# include "minirt_colors.h"
# include "minirt_hooks.h"
# include "libft.h"

//stores data like colors, intersection points etc.
typedef struct	s_intersection
{

}				t_intersection;

typedef struct	s_ray
{
	
}				t_ray;

typedef struct	s_sphere
{
	float		radius;
}				t_sphere;

typedef struct	s_cylinder
{
	float		radius;
	float		height;
}				t_cylinsder;

typedef struct	s_plane
{
	float		radius;
	float		height;
}				t_plane;

typedef struct	s_object
{
	t_matrix		*transforms;
	t_matrix		*rev_transforms;
	t_matrix		*body;
	t_matrix		*result;
	t_matrix		*projection;
	t_matrix		*center;
	t_matrix		*t_center;
	t_matrix		*rev_t_center;
	t_matrix		*tmp;

	void			*object_shape;
	//todo: set an intersection function pointer.
	int				(*intersect)(void *shape, t_object *object, t_ray *ray);
	int				(*transform)(void *shape, t_matrix *transform);
	float			*colors;
	int				light;
	float			base_color;
	float			intensity;
	float			lightness;
	float			light_base;
	float			saturation;
	float			sat_base;
	int				width;
	int				height;
	int				shape[2];
}				t_object;

typedef struct	s_transforms_3d
{
	t_matrix		*x_axis;
	t_matrix		*y_axis;
	t_matrix		*z_axis;
}				t_transforms_3d;

typedef struct s_display
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				width;
	int				height;
}					t_display;

typedef struct	s_minirt
{
	void			*mlx;
	void			*window;

	t_transforms_3d	*rotations;
	t_transforms_3d	*translations;
	t_transforms_3d	*rev_rotations;
	t_transforms_3d	*rev_translations;
	t_transforms_3d	*scalings;
	t_transforms_3d	*rev_scalings;

	t_matrix		*transforms;
	t_matrix		*centering;
	t_matrix		*tmp;
	t_matrix		*zooming_in;
	t_matrix		*zooming_out;

	t_darray		*objects;
	t_object		*target;

	t_display		*display;
}				t_minirt;

int	handle_expose(t_minirt *minirt);
int	close_program(t_minirt *minirt);
int	update_view(t_minirt *minirt);

#endif