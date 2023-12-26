/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotations.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/26 14:30:46 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdio.h>

int	x_rotation(t_matrix *matrix, float angle)
{
	angle = to_rad(angle);
	matrix->points[0][0] = 1.0f;
	matrix->points[1][1] = cosf(angle);
	matrix->points[1][2] = sinf(angle);
	matrix->points[2][1] = -sinf(angle);
	matrix->points[2][2] = cosf(angle);
	return (0);
}

int	y_rotation(t_matrix *matrix, float angle)
{
	angle = to_rad(angle);
	matrix->points[0][0] = cosf(angle);
	matrix->points[0][2] = -sinf(angle);
	matrix->points[1][1] = 1.0f;
	matrix->points[2][0] = sinf(angle);
	matrix->points[2][2] = cosf(angle);
	return (0);
}

t_matrix	*z_rotation(t_matrix *matrix, float angle)
{
	angle = to_rad(angle);
	matrix->points[0][0] = cosf(angle);
	matrix->points[0][1] = -sinf(angle);
	matrix->points[1][0] = sinf(angle);
	matrix->points[1][1] = cosf(angle);
	matrix->points[2][2] = 1.0f;
	return (0);
}
