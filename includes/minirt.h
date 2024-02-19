/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:24:06 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/19 18:16:46 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include "linear_algebra.h"
# include "minirt_bindings.h"
# include "minirt_colors.h"
# include "libft.h"
# include "mlx.h"
# include "minirt_controls.h"
# include "get_next_line.h"
# include "minirt_objects.h"

# define FLT_MAX 3.402823466e+38F

typedef struct s_light
{
	float	color[4];
	float	direction[4];
	float	position[4];
	float	brightness;
}	t_light;

typedef struct s_camera
{
	t_matrix		basis;
	t_matrix		view;
	t_matrix		inverse_view;
	float			orientation[4];
	float			origin[4];
	float			*position;
	float			fov;
	float			near;
	float			far;
}				t_camera;

typedef struct s_transforms_3d
{
	t_matrix		x_axis;
	t_matrix		y_axis;
	t_matrix		z_axis;
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
}					t_display;

typedef struct s_minirt
{
	void			*mlx;
	void			*window;

	t_transforms_3d	rotations;
	t_transforms_3d	rev_rotations;

	t_matrix		world_space;
	t_matrix		tmp;
	t_object		*selected_object;

	t_light			ambient;

	t_darray		lights;
	t_darray		objects;
	t_darray		materials;
	t_darray		perturbators;
	t_darray		textures;

	t_display		display;
	t_camera		camera;
	t_mouse			mouse;
	t_key			shift;
	t_key			ctrl;
	t_key			alt;

	float			sky_color[4];
	int				key_pressed;
	int				light_index;
	int				(*render_mode)(struct s_minirt*);
	int				(*low_res)(struct s_minirt*);
	int				(*high_res)(struct s_minirt*);
}				t_minirt;

int		minirt_init(t_minirt *minirt);
int		minirt_cleanup(t_minirt *minirt);
int		close_program(t_minirt *minirt);

int		key_release_hook(int code, t_minirt *minirt);
int		key_press_hook(int code, t_minirt *minirt);

int		set_rotations(t_minirt *fdf, float x, float y, float z);
int		set_basis(t_matrix *basis, float orientation[3]);
int		set_camera_transform(t_minirt *minirt, t_camera *camera);
int		look_at(t_camera *camera, t_minirt *fdf);
int		perspective_projector(t_matrix *matrix,
			t_display *display, t_camera *camera);
void	set_translate(t_matrix *matrix, float x, float y, float z);
float	to_rad(float degrees);
float	project_point(t_ray *ray, t_object *object, float hit);

int		mouse_click_hook(int button, int x, int y, t_minirt *minirt);
int		mouse_release_hook(int button, int x, int y, t_minirt *minirt);
int		mouse_update(int x, int y, t_minirt *minirt);

float	*to_screen_space(t_display *display, float pixel[4], float i, float j);
float	*to_world_space(t_minirt *minirt, float point[4], float result[4]);
void	create_basis(float orientation[3], float u[3], float v[3]);

int		low_resolution(t_minirt *minirt);
int		full_resolution(t_minirt *minirt);

int		load_bmaps(t_minirt *minirt);
int		set_materials(t_minirt *minirt);

int		grab_object(t_minirt *minirt, int x, int y);

int		rotate_x(t_minirt *minirt, t_matrix *matrix, float dir[3]);
int		rotate_y(t_minirt *minirt, t_matrix *matrix, float dir[3]);
int		rotate_z(t_minirt *minirt, t_matrix *matrix, float dir[3]);
int		rotate_object(t_minirt *minirt, t_object *object, t_matrix *axis);

void	apply_translation(t_minirt *minirt, float dir[4], float speed);
void	apply_scaling(t_minirt *minirt, float dir[4]);
void	apply_rotation(t_minirt *minirt, float dir[4]);

int		update_material(t_minirt *minirt, t_object *object,
			float direction[4]);

int		control_camera(int code, t_minirt *minirt);

int		handle_light(t_minirt *minirt, t_hit *hit,
			t_light *light, float color[3]);
int		ray_trace(t_minirt *minirt, t_ray *ray, t_hit *hit, int coords[2]);
int		shade_pixel(t_minirt *minirt, int coords[2]);
int		add_textures(t_minirt *minirt, t_hit *hit, float color[4]);
void	set_ray(t_minirt *minirt, t_ray *ray, int x, int y);
void	bounce_ray(t_ray *ray, t_hit *hit);
void	cast_shadows(t_minirt *minirt, t_hit *hit,
			t_light *light, float visibility[3]);

float	*reflect(float incident[3], float normal[3],
			float hit_angle, float result[3]);

void	minirt_pixel_put(t_display *display, int x, int y, int color);
int		render(t_minirt *minirt);

#endif
