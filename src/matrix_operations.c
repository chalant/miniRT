/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/29 15:53:50 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	matrix_copy(t_matrix *source, t_matrix *dest, int n)
{
	int	i;
	int	j;

	i = -1;
	while (++i < n)
	{
		j = -1;
		while (++j < n)
			dest->points[i][j] = source->points[i][j];
	}
}

int	smatmul(t_matrix *mat, float scalar, t_matrix *result)
{
	int	i;
	int	j;

	i = -1;
	while (++i < mat->rows)
	{
		j = -1;
		while (++j < mat->cols)
			result->points[i][j] = scalar * mat->points[i][j];
	}
	return (1);
}

int	vmatmul(t_matrix *mat, float *vec, float *result)
{
	int	i;
	int	j;

	i = -1;
	while (++i < mat->rows)
	{
		j = -1;
		while (++j < mat->cols)
			result[i] += vec[j] * mat->points[i][j];
	}
	return (1);
}

void	matmul(t_matrix *mat1, t_matrix *mat2, t_matrix *result, int n)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	while (++i < n)
	{
		j = -1;
		while (++j < n)
		{
			result->points[i][j] = 0.0;
			k = -1;
			while (++k < n)
				result->points[i][j] += mat1->points[i][k] * mat2->points[k][j];
		}
	}
}

void	mattranspose(t_matrix *matrix, t_matrix *result)
{
	int	i;
	int	j;

	i = -1;
	while (++i < matrix->rows)
	{
		j = -1;
		while (++j < matrix->cols)
			result->points[j][i] = matrix->points[i][j];
	}
}

void	inplace_matmul(t_matrix *mat1, t_matrix *mat2, t_matrix *result, int n)
{
	matmul(mat1, mat2, result, n);
	matrix_copy(result, mat2, n);
}
