/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:08:46 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:55:37 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	delete_perturbator(void *address)
{
	t_perturbator	*perturbator;

	perturbator = address;
	delete_matrix(&perturbator->map);
}

int	minirt_cleanup(t_minirt *minirt)
{
	delete_matrix(&minirt->mouse.direction);
	delete_matrix(&minirt->camera.basis);
	delete_matrix(&minirt->camera.view);
	delete_matrix(&minirt->camera.inverse_view);
	ft_darray_delete(&minirt->objects, NULL);
	ft_darray_delete(&minirt->lights, NULL);
	ft_darray_delete(&minirt->textures, NULL);
	ft_darray_delete(&minirt->materials, NULL);
	ft_darray_delete(&minirt->perturbators, NULL);
	return (1);
}
