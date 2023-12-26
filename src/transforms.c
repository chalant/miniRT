/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transforms.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/29 15:53:50 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_matrix	*translation(t_matrix *matrix, float x, float y, float z)
{
	matrix->points[0][3] = x;
	matrix->points[1][3] = y;
	matrix->points[2][3] = z;
	matrix->points[0][0] = 1.0;
	matrix->points[1][1] = 1.0;
	matrix->points[2][2] = 1.0;
	return (0);
}

int	scaling(t_matrix *matrix, float x, float y, float z)
{
	matrix->points[0][0] = x;
	matrix->points[1][1] = y;
	matrix->points[2][2] = z;
	return (0);
}
