/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:05:32 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	create_matrix(t_matrix *matrix, int rows, int cols)
{
	int	i;

	if (cols < 0 || rows < 0)
		return (-1);
	matrix->points = malloc(rows * sizeof(float *));
	if (!matrix->points)
		return (-1);
	i = -1;
	while (++i < rows)
		matrix->points[i] = NULL;
	matrix->cols = cols;
	matrix->rows = rows;
	return (0);
}

int	delete_matrix(t_matrix *matrix)
{
	int	i;

	if (!matrix)
		return (0);
	if (!matrix->points)
		return (0);
	i = -1;
	while (++i < matrix->rows)
		free(matrix->points[i]);
	free(matrix->points);
	return (0);
}

int	fill_vector(float *vector, int size, float value)
{
	int	i;

	if (!vector)
		return (-1);
	i = 0;
	while (i < size)
	{
		vector[i] = value;
		i++;
	}
	return (0);
}

int	init_matrix(t_matrix *matrix, float value)
{
	int			i;

	matrix->points[0] = malloc(matrix->cols * sizeof(float));
	if (!matrix->points[0])
		return (-1);
	fill_vector(matrix->points[0], matrix->cols, value);
	i = 1;
	while (matrix->points[i - 1] && i < matrix->rows)
	{
		matrix->points[i] = malloc(matrix->cols * sizeof(float));
		fill_vector(matrix->points[i], matrix->cols, value);
		i++;
	}
	if (!matrix->points[i - 1])
		return (-1);
	return (0);
}

int	homogeneous_matrix(t_matrix *matrix, int rows, int cols)
{
	create_matrix(matrix, rows + 1, cols + 1);
	init_matrix(matrix, 0.0f);
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
