/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:08:46 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/26 17:11:03 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	delete_perturbator(void *address)
{
	t_perturbator	*perturbator;

	perturbator = address;
	delete_matrix(&perturbator->map);
}

void	delete_object(void *address)
{
	t_object	*object;

	object = address;
	delete_matrix(&object->basis);
}

int	minirt_cleanup(t_minirt *minirt)
{
	delete_matrix(&minirt->tmp);
	delete_matrix(&minirt->world_space);
	delete_matrix(&minirt->mouse.direction);
	delete_matrix(&minirt->camera.basis);
	delete_matrix(&minirt->camera.view);
	delete_matrix(&minirt->camera.inverse_view);
	delete_matrix(&minirt->rotations.x_axis);
	delete_matrix(&minirt->rotations.y_axis);
	delete_matrix(&minirt->rotations.z_axis);
	delete_matrix(&minirt->rev_rotations.x_axis);
	delete_matrix(&minirt->rev_rotations.y_axis);
	delete_matrix(&minirt->rev_rotations.z_axis);
	ft_darray_delete(&minirt->objects, delete_object);
	ft_darray_delete(&minirt->lights, NULL);
	ft_darray_delete(&minirt->textures, NULL);
	ft_darray_delete(&minirt->materials, NULL);
	ft_darray_delete(&minirt->perturbators, delete_perturbator);
	return (1);
}
