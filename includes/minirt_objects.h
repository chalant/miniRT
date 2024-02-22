/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_objects.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:06:34 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/22 12:34:44 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_OBJECTS_H
# define MINIRT_OBJECTS_H

# define PLANE 0

typedef struct s_ray
{
	float		object_center[4];
	float		direction[4];
	float		origin[3];
	float		closest_t;
	float		t;
}	t_ray;

typedef struct s_material
{
	float		diffuse_reflection;
	float		ambient_reflection;
	float		specular_reflection;
	float		shininess;
	float		emission;
	float		reflectivity;
	float		color[4];
	float		repeat_pattern;
}		t_material;

typedef struct s_hit
{
	struct s_object		*object;
	struct s_material	*material;
	float				*ray_origin;
	int					inside;
	float				color[4];
	float				normal[3];
	float				point[3];
	float				distance;
	float				energy;
	int					*screen_coords;
}		t_hit;

typedef struct s_object
{
	t_matrix		basis;
	float			center[4];
	int				id;
	int				(*intersect)(struct s_object*, t_ray*);
	int				(*transform)(struct s_object*, t_matrix*, float*);
	float			*(*normal)(struct s_object*, t_hit*);
	float			*(*uv_coords)(struct s_object*, t_hit*, float coords[2]);
	float			color[4];
	float			orientation[3];
	float			size[3];
	int				material_index;
	int				perturbator_index;
	int				texture_index;
}				t_object;

typedef struct s_texture
{
	float	*(*get_texture)(struct s_material*, struct s_object*,
			float uv_coords[2], float color[4]);
}	t_texture;

typedef struct s_perturbator
{
	t_matrix	map;

	float		*(*perturb_normal)(struct s_perturbator*, struct s_object*,
			float uv_coords[2], float bump[3]);
}	t_perturbator;

int		create_sphere(t_object *object, float radius);
int		create_plane(t_object *object, float normal[4]);
int		create_cone(t_object *object, float height, float radius);
int		create_cylinder(t_object *object, float height, float radius);

float	*checkerboard(t_material *material, t_object *object,
			float uv_coords[2], float color[4]);
float	*horizontal_bands(t_material *material, t_object *object,
			float uv_coords[2], float color[4]);
float	*vertical_bands(t_material *material, t_object *object,
			float uv_coords[2], float color[4]);
float	*no_texture(t_material *material, t_object *object,
			float uv_coords[2], float color[4]);
float	*compute_bump(t_perturbator *perturbator, t_object *object,
			float uv_coords[2], float bump[3]);
float	*no_perturbation(t_perturbator *perturbator, t_object *object,
			float uv_coords[2], float bump[3]);

#endif
