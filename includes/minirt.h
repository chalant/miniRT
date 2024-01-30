#ifndef MINIRT_H
# define MINIRT_H

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include "matrix.h"
# include "minirt_bindings.h"
# include "minirt_colors.h"
# include "libft.h"
# include "mlx.h"
# include "minirt_controls.h"

typedef struct	s_light
{
	float	color[4];
	float	direction[4];
	float	position[4];
	float	brightness;
}	t_light;

typedef struct	s_ray
{
	float		object_center[4];
	float		direction[4];
	float		*origin;
	float		closest_t;
	float		t;
}				t_ray;

typedef struct	s_camera
{
	t_matrix		*basis;
	t_matrix		*view;
	t_matrix		*inverse_view;
	float			*orientation;
	float			right[3];
	float			up[3];
	float			origin[4];
	float			fov;
	float			near;
	float			far;
}				t_camera;

typedef struct	s_sphere
{
	float		radius;
	const char	*name;
}				t_sphere;

typedef struct	s_cylinder
{
	float		radius;
	float		height;
}				t_cylinsder;

typedef struct	s_plane
{
	float		normal[4];
	float		point[4];
}				t_plane;

typedef struct	s_hit
{
	struct	s_object	*object;
	float				*ray_origin;
	float				color[4];
	float				normal[3];
	float				point[3];
	float				distance;
	int					*screen_coords;
}	t_hit;

typedef struct	s_material
{
	float	diffuse_reflection;
	float	ambient_reflection;
	float	shininess;
	float	color[4];
	float	dark_color[4];
	float	roughness;
	float	emission;
}	t_material;

typedef struct	s_object
{
	t_matrix		*body;
	t_matrix		*result;
	t_matrix		*tmp;
	t_material		*material;

	const char		*name;
	void			*shape;
	float			*center;
	float			albedo;
	int				id;
	int				(*intersect)(struct s_object*, t_ray*);
	int				(*transform)(struct s_object*, t_matrix*, float*);
	float			*(*normal)(struct s_object*, t_hit*);
	float			*(*uv_coord)(struct s_object, t_hit*);
	float			color[4];
	int				width;
	int				height;
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
	int				offset;
	int				line_length;
	int				endian;
	int				width;
	int				height;
	float			aspect_ratio;
	float			*origin;
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

	t_matrix		*view_matrix;
	t_matrix		*world_space;
	t_matrix		*screen_space;
	t_matrix		*transforms;
	t_matrix		*centering;
	t_matrix		*tmp;

	t_light			*light;
	t_light			diffuse;
	t_light			ambient;
	t_darray		*spot_lights;
	t_darray		*objects;
	t_object		*target;

	t_display		*display;
	t_camera		*camera;
	t_mouse			mouse;

	float			x[4];
	float			y[4];
	float			z[4];

	int				rendered;
}				t_minirt;

int		minirt_init(t_minirt *minirt);
int		handle_expose(t_minirt *minirt);
int		close_program(t_minirt *minirt);
int		update_view(t_minirt *minirt);

int		key_release_hook(int code, t_minirt *minirt);
int		key_press_hook(int code, t_minirt *minirt);
void	fdf_control_key(int code, t_minirt *fdf);
int		color_hook(int code, t_minirt *fdf);
void	movement_hook(int code, t_minirt *fdf);

int		set_rotations(t_minirt *fdf, float x, float y, float z);
int		translation(t_matrix *matrix, float x, float y, float z);
int		set_translations(t_minirt *fdf, float x, float y, float z);

float	to_rad(float degrees);
int		set_camera_transform(t_camera *camera);
int		look_at(t_camera *camera, t_minirt *fdf);
int		perspective_projector(t_matrix **matrix, t_display *display, t_camera *camera);
int		render(t_minirt *minirt);
void	set_translate(t_matrix *matrix, float x, float y, float z);

#endif