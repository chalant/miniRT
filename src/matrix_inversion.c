/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_inversion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:05:42 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:05:43 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	set_matrix(t_matrix *matrix, float value)
{
	int	i;
	int	j;

	i = -1;
	while (++i < matrix->rows)
	{
		j = -1;
		while (++j < matrix->cols)
			matrix->points[i][j] = value;
	}
	return (0);
}

int	compute_pivots(t_matrix *matrix, t_matrix *identity, int i, int n)
{
	int			j;
	float		diag;

	j = -1;
	diag = matrix->points[i][i];
	if (diag == 0.0f)
		return (0);
	while (++j < n)
	{
		matrix->points[i][j] /= diag;
		identity->points[i][j] /= diag;
	}
	return (1);
}

int	invert_init(t_matrix *matrix, t_matrix *result, t_matrix *identity, int n)
{
	set_matrix(identity, 0.0f);
	set_diagonal(identity, 1.0f);
	matrix_copy(matrix, result, n);
	return (-1);
}

void	invert_matrix(t_matrix *matrix, t_matrix *result,
		t_matrix *identity, int n)
{
	float		factor;
	int			i;
	int			j;
	int			k;

	i = invert_init(matrix, result, identity, n);
	while (++i < n)
	{
		if (!compute_pivots(result, identity, i, n))
			return ;
		k = -1;
		while (++k < n)
		{
			if (k == i)
				continue ;
			factor = result->points[k][i];
			j = -1;
			while (++j < n)
			{
				result->points[k][j] -= factor * result->points[i][j];
				identity->points[k][j] -= factor * identity->points[i][j];
			}
		}
	}
	matrix_copy(identity, result, n);
}
