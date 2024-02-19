#include "minirt.h"

int	set_textures(t_minirt *minirt)
{
	t_texture	txt;

	txt.get_texture = no_texture;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = checkerboard;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = horizontal_bands;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = vertical_bands;
	ft_darray_append(&minirt->textures, &txt);
	return (0);
}

int	init_material(t_material *material)
{
	material->ambient_reflection = 0.2f;
	material->diffuse_reflection = 0.5f;
	material->specular_reflection = 0.5f;
	material->shininess = 10.0f;
	material->reflectivity = 0.0f;
	material->repeat_pattern = 1.0f;
	to_color(0x00ffffff, material->color);
	return (0);
}

int	set_materials(t_minirt *minirt)
{
	int			i;
	t_material	material;
	t_object	*object;

	set_textures(minirt);
	init_material(&material);
	i = -1;
	while (++i < minirt->objects.size)
	{
		object = ft_darray_get(&minirt->objects, i);
		object->material_index = i;
		object->texture_index = 0;
		object->perturbator_index = 0;
		if (object->id == PLANE)
			material.repeat_pattern = 0.01f;
		else
			material.repeat_pattern = 8.0f;
		if (ft_darray_append(&minirt->materials, &material))
			return (-1);
	}
	return (0);
}
