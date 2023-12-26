/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_interface.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/29 15:53:50 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	get_point(t_matrix *matrix, int i, int j)
{
	return (matrix->points[i][j]);
}

void	set_point(t_matrix *matrix, int i, int j, float value)
{
	matrix->points[i][j] = value;
}

float	**get_points(t_matrix *matrix)
{
	return (matrix->points);
}
