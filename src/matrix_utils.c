/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:09:20 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 17:09:34 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

int	homogeneous_matrix(t_matrix *matrix, int rows, int cols)
{
	if (create_matrix(matrix, rows + 1, cols + 1) < 0)
		return (-1);
	if (init_matrix(matrix, 0.0f) < 0)
		return (-1);
	matrix->points[rows][cols] = 1.0f;
	return (0);
}

int	set_diagonal(t_matrix *matrix, float value)
{
	int	i;
	int	target;

	i = -1;
	if (matrix->rows <= matrix->cols)
		target = matrix->rows;
	else
		target = matrix->cols;
	while (++i < target)
		matrix->points[i][i] = value;
	return (0);
}
