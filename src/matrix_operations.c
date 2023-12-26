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

void	matrix_copy(t_matrix *source, t_matrix *dest)
{
	int	i;
	int	j;

	i = -1;
	while (++i < source->rows)
	{
		j = -1;
		while (++j < source->cols)
			dest->points[i][j] = source->points[i][j];
	}
}

void	map_row(t_matrix *matrix, void (*op)(float *, float), int index,
		float value)
{
	int	j;

	j = -1;
	while (++j < matrix->cols)
		op(&matrix->points[index][j], value);
}

void	map_col(t_matrix *matrix, void (*op)(float *, float), int index,
		float value)
{
	int	j;

	j = -1;
	while (++j < matrix->rows)
		op(&matrix->points[j][index], value);
}

void	matmul(t_matrix *mat1, t_matrix *mat2, t_matrix *result)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	while (++i < mat1->rows)
	{
		j = -1;
		while (++j < mat2->cols)
		{
			result->points[i][j] = 0.0;
			k = -1;
			while (++k < mat1->cols)
				result->points[i][j] += mat1->points[i][k] * mat2->points[k][j];
		}
	}
}

void	inplace_matmul(t_matrix *mat1, t_matrix *mat2, t_matrix *result)
{
	matmul(mat1, mat2, result);
	matrix_copy(result, mat2);
}
