/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/12 15:31:32 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "map.h"
#include "minirt_controls.h"
#include <stdio.h>

int	set_materials(t_minirt *minirt)
{
	t_material	material;
	t_texture	txt;
	t_object	*object;

	material.ambient_reflection = 0.2f;
	material.diffuse_reflection = 0.5f;
	material.specular_reflection = 0.5f;
	material.shininess = 10.0f;
	material.reflectivity = 0.0f;
	material.repeat_pattern = 1.0f;
	to_color(0x00ffffff, material.color);
	txt.get_texture = no_texture;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = checkerboard;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = horizontal_bands;
	ft_darray_append(&minirt->textures, &txt);
	txt.get_texture = vertical_bands;
	ft_darray_append(&minirt->textures, &txt);
	int	i;

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
		ft_darray_append(&minirt->materials, &material);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_minirt	minirt;

	if (argc != 2)
		return (1);
	if (minirt_init(&minirt) < -1)
		return (1);
	if (import_map(&minirt, argv))
		return (minirt_cleanup(&minirt) * 1);
	if (set_camera_transform(&minirt, &minirt.camera) == -1)
		return (minirt_cleanup(&minirt));
	set_materials(&minirt);
	if (load_bmaps(&minirt) < 0)
	{
		printf("FAIL\n");
		return (minirt_cleanup(&minirt));
	}
	mlx_loop(minirt.mlx);
	return (0);
}
